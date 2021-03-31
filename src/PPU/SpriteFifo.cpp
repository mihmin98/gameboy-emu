#include "SpriteFifo.hpp"

SpriteFifo::SpriteFifo() {}

SpriteFifo::SpriteFifo(PPU *ppu) : ppu(ppu) {}

FifoPixel *SpriteFifo::cycle(BgFifo &bgFifo)
{
    FifoPixel *returnedPixel = nullptr;

start:
    // Check for sprite at xPos if not currently fetching sprite
    if (!fetchingSprite && !(ppu->emulatorMode == DMG && !ppu->getObjDisplayEnable())) {
        for (int i = 0; i < ppu->numSpritesOnCurrentLine; ++i) {
            int8_t spriteIndex = ppu->spritesOnCurrentLine[i];
            if (spriteIndex == -1 || processedSprites.find(spriteIndex) != processedSprites.end())
                continue;

            OAMSprite sprite = ppu->getSpriteByIndex(spriteIndex);

            if (sprite.xPos - 8 == fetcherXPos || (sprite.xPos - 8 < 0 && fetcherXPos == 0) ||
                (sprite.xPos - 8 > 160 && fetcherXPos == 160)) {

                currentSpriteIndex = spriteIndex;
                spriteIndexInFoundSprites = i;
                fetchingSprite = true;
                fetchedSprite = false;
                fetcherStep = 0;
                fetcherStage = 1;
                processedSprites.insert(currentSpriteIndex);
            }
        }
    }

    if (fetchingSprite) {
        // Apply penalty for sprite at xpos = 0
        if (fetcherStep == 0 && fetcherXPos == 0 && !appliedPenaltyAtXPos0) {
            uint8_t penalty = ppu->getScrollX() % 8;

            oamPenalty += penalty;
            ppu->drawModeLength += penalty;
            ppu->hBlankModeLength -= penalty;

            appliedPenaltyAtXPos0 = true;

            checkForAbort = true;
            goto checkAbort;
        }

        // Stage 1
        if (fetcherStage == 1 && (fetcherStep < 5 || bgFifo.pixelQueue.empty())) {
            fetcherStep++;
            oamPenalty += 1;
            ppu->drawModeLength += 1;
            ppu->hBlankModeLength -= 1;

            checkForAbort = true;
            goto checkAbort;
        } else if (fetcherStage == 1) {
            fetcherStage++;
        }

        // Stage 2
        if (fetcherStage == 2) {
            fetcherStep++;
            fetcherStage++;

            oamPenalty += 1;
            ppu->drawModeLength += 1;
            ppu->hBlankModeLength -= 1;

            checkForAbort = true;
            goto checkAbort;
        }

        // Stage 3
        if (fetcherStage == 3) {
            fetcherStep++;
            fetcherStage++;

            oamPenalty += 3;
            ppu->drawModeLength += 3;
            ppu->hBlankModeLength -= 3;

            checkForAbort = true;
            goto checkAbort;
        }

        // Stage 4
        if (fetcherStage == 4) {
            fetcherStep++;
            fetcherStage++;

            oamPenalty += 3;
            ppu->drawModeLength += 3;
            ppu->hBlankModeLength -= 3;

            checkForAbort = true;
            goto checkAbort;
        }

        // Stage 5 (get low line address)
        if (fetcherStage == 5) {
            fetcherStep++;
            fetcherStage++;

            oamPenalty += 1;
            ppu->drawModeLength += 1;
            ppu->hBlankModeLength -= 1;

            checkForAbort = true;
            goto checkAbort;
        }

        // Stage 6 (end fetch)
        if (fetcherStage == 6) {
            fetcherStep++;
            fetcherStage++;

            oamPenalty += 1;
            ppu->drawModeLength += 1;
            ppu->hBlankModeLength -= 1;

            checkForAbort = false;
            goto checkAbort;
        }

        // Stage 7 (get sprite)
        if (fetcherStage == 7) {
            OAMSprite sprite = ppu->getSpriteByIndex(currentSpriteIndex);

            // Get tile
            uint8_t screenLine = ppu->getLy();
            uint8_t tileNo = 0;
            uint8_t vramBank = ppu->emulatorMode == DMG ? 0 : sprite.tileVramBank;

            if (ppu->getObjSize() == 1) {
                // 8x16
                tileNo = (screenLine - sprite.yPos + 16) / 8;
                if (sprite.yFlip)
                    tileNo = !tileNo;
            }

            Tile tile = ppu->getSpriteTile(sprite.tileNumber, tileNo, vramBank);

            // Flip tile if necessary
            if (sprite.xFlip)
                tile.flipHor();

            if (sprite.yFlip)
                tile.flipVert();

            // Get row from tile
            uint8_t tileRow = (screenLine - sprite.yPos + 16) % 8;
            uint8_t tileRowData[8];

            tile.getTileRow(tileRow, tileRowData);

            // Set pixels
            FifoPixel pixels[8];
            for (int i = 0; i < 8; ++i) {
                pixels[i].isSprite = true;
                pixels[i].color = tileRowData[i];
                if (ppu->emulatorMode == DMG) {
                    pixels[i].palette = sprite.dmgPaletteNumber;
                } else {
                    pixels[i].palette = sprite.cgbPaletteNumber;
                    pixels[i].spritePriority = currentSpriteIndex;
                    pixels[i].bgPriority = sprite.objToBgPriority;
                }
            }

            // Check for previous pixels and do the mixing
            std::vector<FifoPixel> pixelQueueVector;
            while (!pixelQueue.empty()) {
                pixelQueueVector.push_back(pixelQueue.front());
                pixelQueue.pop();
            }

            for (int i = 0; i < 8; ++i) {
                if (pixelQueueVector.size() == i) {
                    // If there are not enough elements, just add the pixel
                    pixelQueueVector.push_back(pixels[i]);
                } else {
                    // Check if the pixel is transparent, if yes then replace
                    if (pixelQueueVector[i].color == 0)
                        pixelQueueVector[i] = pixels[i];
                }
            }

            for (int i = 0; i < 8; ++i)
                pixelQueue.push(pixelQueueVector[i]);

            // Discard offscreen pixels and add transparent pixels after
            if (fetcherXPos == 0 && sprite.xPos - 8 < 0) {
                uint8_t discardedPixels = 8 - sprite.xPos;
                FifoPixel blankPixel = FifoPixel(0, 0, 41, sprite.objToBgPriority, true);
                for (int i = 0; i < discardedPixels; ++i) {
                    pixelQueue.pop();
                    pixelQueue.push(blankPixel);
                }
            }

            // Check for next sprite at current xpos? here or after checkabort?
            fetchedSprite = true;
            fetchingSprite = false;
            goto start;
        }
    }

checkAbort:
    if (checkForAbort) {
        // TODO: Check how abort works on the CGB
        abortFetch = ppu->getObjDisplayEnable();

        checkForAbort = false;
    }

    // Abort fetch also happens after fetching the sprite
    // TODO: Check penalty when aborting
    if (abortFetch || fetchedSprite) {
        abortFetch = false;
        fetchedSprite = false;
        fetchingSprite = false;

        // TODO: Check if these 2 should be here
        fetcherStage = 1;
        fetcherStep = 0;

        oamPenalty += 1;
        ppu->drawModeLength += 1;
        ppu->hBlankModeLength -= 1;
    }

    // If not fetching and queue is not empty then push pixel
    if (!fetchingSprite && pixelQueue.size() > 0) {
        returnedPixel = new FifoPixel(pixelQueue.front());
        pixelQueue.pop();
    }

    return returnedPixel;
}

void SpriteFifo::prepareForLine()
{
    abortFetch = false;
    fetchedSprite = false;
    fetchingSprite = false;
    fetcherXPos = 0;
    checkForAbort = false;
    fetcherStage = 0;
    fetcherStep = 0;

    currentSpriteIndex = 0;
    spriteIndexInFoundSprites = 0;

    oamPenalty = 0;

    while (!pixelQueue.empty())
        pixelQueue.pop();

    processedSprites.clear();
}

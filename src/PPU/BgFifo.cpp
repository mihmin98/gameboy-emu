#include "BgFifo.hpp"

BgFifo::BgFifo() {}

BgFifo::BgFifo(PPU *ppu) : ppu(ppu) {}

FifoPixel *BgFifo::cycle()
{
    FifoPixel *returnedPixel = nullptr;

    // TODO: daca exista scrolling pe tile-ul curent (SCX % 8 != 0) si Wx = 0, atunci se scurteaza
    // Mode 3 cu 1 T-cycle

    // check for window
    if (ppu->getWindowDisplayEnable() && coordsInsideWindow(pushedPixels, ppu->getLy())) {
        isDrawingWindow = true;

        // clear the queue
        clearQueue();

        // set the fetcher stage and position
        fetcherStage = GET_TILE;
        fetcherStageCycles = 0;
        fetcherXPos = pushedPixels;
    }

    cycleFetcher();

    // check if there are pixels to push
    if (pixelQueue.size() > 1) {
        FifoPixel pixel = pixelQueue.front();
        pixelQueue.pop();

        // check if it needs to discard pixels
        if (scxPixelsToDiscard > 0) {
            --scxPixelsToDiscard;
        } else {
            returnedPixel = new FifoPixel();
            *returnedPixel = pixel;
            ++pushedPixels;
        }
    }

    return returnedPixel;
}

void BgFifo::cycleFetcher()
{
    switch (fetcherStage) {
    case GET_TILE:
        if (fetcherStageCycles < 2)
            break;

        // If in dmg mode and bg and window are disabled, then do nothing; a row of blank pixels
        // will be pushed in the PUSH stage
        if (ppu->getBgWindowDisplayPriority() == 0 && ppu->emulatorMode == DMG)
            break;

        if (isDrawingWindow) {
            tilemapBaseAddr = ppu->getWindowTileMapDisplaySelect() == 0 ? 0x9800 : 0x9C00;

            // done? TODO: This is not the tile pos for the window; window se deseneaza la fel ca
            // bg, probabil fara scroll ar fi fetcher coord - window coord? screen x pos e pozitia
            // pixelului x de pe ecran screen y pos e pozitia pixelului y de pe ecran ca sa aflu
            // tile-ul din window: fac scaderea ca sa aflu coord din window;
            tileXPos = (fetcherXPos - ppu->getWx() - 7) / 8;
            tileYPos = (fetcherYPos - ppu->getWy()) / 8;
            // done? TODO: Window X pos is actually x pos + 7, need to test this
            // the window is drawn from Wx - 7

        } else {
            tilemapBaseAddr = ppu->getBgTileMapDisplaySelect() == 0 ? 0x9800 : 0x9C00;

            tileXPos = ((ppu->getScrollX() + fetcherXPos) / 8) & 0x1F;
            // tileYPos = (fetcherYPos + ppu->getScrollY()) & 0xFF;
            // this should get the line in the 256x256 screen
            tileYPos = ((ppu->getScrollY() + fetcherYPos) / 8) & 0x1F;
        }

        fetcherStage = GET_TILE_DATA_LOW;
        fetcherStageCycles = 0;
        break;

    case GET_TILE_DATA_LOW:
        if (fetcherStageCycles < 2)
            break;
        // do nothing here because we retrieve the entire tile at the same time
        break;

    case GET_TILE_DATA_HIGH:
        if (fetcherStageCycles < 2)
            break;
        // do nothing here because we retrieve the entire tile at the same time
        break;

    case SLEEP:
        // check if there is room in the fifo to push pixels, otherwise do nothing
        if (pixelQueue.size() <= 8) {
            fetcherStage = PUSH;
            fetcherStageCycles = 0;
        }

        break;

    case PUSH:
        // pushes the pixels in the queue
        // TODO: Check if the entire row is pushed in 1 cycle

        // if in DMG mode and not drawing bg or window, then add a row of blank pixels
        if (ppu->getBgWindowDisplayPriority() == 0 && ppu->emulatorMode == DMG) {
            FifoPixel blankPixel = FifoPixel(0, 0, 0, 0, false);
            for (int i = 0; i < 8; ++i) {
                pixelQueue.push(blankPixel);
            }
            break;
        }

        int tileMapIndex = tileYPos * 32 + tileXPos;
        uint8_t tileIndex = ppu->memory->readmem(tilemapBaseAddr + tileMapIndex);

        tile = ppu->getTileByIndex(tileIndex);

        // if in CGB mode, get bg map attributes and flip the tile if necessary
        BgMapAttributes bgMapAttr;
        if (ppu->emulatorMode == CGB) {
            bgMapAttr = ppu->getBgMapByIndex(tileMapIndex, tilemapBaseAddr == 0x9800 ? 0 : 1);
            if (bgMapAttr.horizontalFlip)
                tile.flipHor();
            if (bgMapAttr.verticalFlip)
                tile.flipVert();
        }

        // get the row of pixels from the tile
        uint8_t tileRow[8];

        // TODO: not sure if this is the correct formula for the row
        tile.getTileRow(((fetcherYPos + ppu->getScrollY()) & 0xFF) % 8, tileRow);

        // push the row of pixels into the queue
        for (int i = 0; i < 8; ++i) {
            FifoPixel pixel = FifoPixel(tileRow[i], 0, 0, 0, false);
            if (ppu->emulatorMode == CGB) {
                pixel.palette = bgMapAttr.bgPaletteNumber;
                pixel.bgPriority = bgMapAttr.bgToOamPriority;
            }
            pixelQueue.push(pixel);
        }

        // increase fetcher x pos
        fetcherXPos += 8;

        break;
    }

    ++fetcherStageCycles;
}

void BgFifo::clearQueue()
{
    while (!pixelQueue.empty())
        pixelQueue.pop();
}

bool BgFifo::coordsInsideWindow(uint16_t xPos, uint16_t yPos)
{
    // remember: the window is drawn from Wx - 7
    // screen is 160 wide, 144 tall: so x is between 0..159 and y 0..143
    // we have to render window if screenx >= Wx-7 and screeny >= Wy
    return (xPos >= ppu->getWx() - 7) && (yPos >= ppu->getWy());
}

void BgFifo::resetFetcher(bool resetYPos)
{
    fetcherStage = GET_TILE;
    fetcherStageCycles = 1;
    fetcherXPos = 0;
    isDrawingWindow = false;
    if (resetYPos)
        fetcherYPos = 0;
}

void BgFifo::prepareForLine(uint8_t line)
{
    // set scxPixelsToDiscard
    scxPixelsToDiscard = ppu->getScrollX() % 8;

    pushedPixels = 0;

    // set isDrawingWindow
    isDrawingWindow = false;

    // reset fetcher
    resetFetcher(false);
    fetcherYPos = line;
}
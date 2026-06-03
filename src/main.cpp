#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

int g_clicksInInterval = 0;
float g_timeAccumulator = 0.0f;
CCLabelBMFont* g_cpsLabel = nullptr;

class $modify(MyPlayLayer, PlayLayer) {
    
    bool init(GJGameLevel* level, bool useReplay, bool dontRunLevel) {
        if (!PlayLayer::init(level, useReplay, dontRunLevel)) return false;

        g_clicksInInterval = 0;
        g_timeAccumulator = 0.0f;

        g_cpsLabel = CCLabelBMFont::create("CPS: 0.0", "goldFont.fnt");
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        g_cpsLabel->setPosition({ winSize.width - 55.0f, winSize.height - 15.0f });
        g_cpsLabel->setScale(0.55f);
        g_cpsLabel->setOpacity(190);
        
        this->addChild(g_cpsLabel, 999);

        return true;
    }

    // Usamos una función nativa de actualización que ya existe en PlayLayer para no romper el compilador
    void updateVisibility(float dt) {
        PlayLayer::updateVisibility(dt);

        // Incrementar tiempo usando el delta time del cuadro
        g_timeAccumulator += dt;

        if (g_timeAccumulator >= 4.0f) {
            if (g_cpsLabel) {
                float cps = static_cast<float>(g_clicksInInterval) / g_timeAccumulator;
                std::string cpsText = fmt::format("CPS: {:.1f}", cps);
                g_cpsLabel->setString(cpsText.c_str());
            }
            g_clicksInInterval = 0;
            g_timeAccumulator = 0.0f;
        }
    }

    // Hook alternativo para clics que no usa estructuras de botones raras
    void playStateChanged(int state) {
        PlayLayer::playStateChanged(state);
        // Cada interacción básica o salto incrementa el contador
        g_clicksInInterval++;
    }
};

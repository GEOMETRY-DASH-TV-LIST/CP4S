#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variables de estado globales
int g_clicksInInterval = 0;
float g_timeAccumulator = 0.0f;
CCLabelBMFont* g_cpsLabel = nullptr;

class $modify(MyPlayLayer, PlayLayer) {
    
    bool init(GJGameLevel* level, bool useReplay, bool dontRunLevel) {
        if (!PlayLayer::init(level, useReplay, dontRunLevel)) return false;

        g_clicksInInterval = 0;
        g_timeAccumulator = 0.0f;

        // Crear la etiqueta de texto con la fuente nativa
        g_cpsLabel = CCLabelBMFont::create("CPS: 0.0", "goldFont.fnt");
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // Ubicación en la esquina superior derecha
        g_cpsLabel->setPosition({ winSize.width - 55.0f, winSize.height - 15.0f });
        g_cpsLabel->setScale(0.55f);
        g_cpsLabel->setOpacity(190);
        
        this->addChild(g_cpsLabel, 999);

        return true;
    }

    // Interceptar la actualización nativa de PlayLayer para el temporizador (Evita problemas de selectores)
    void update(float dt) {
        PlayLayer::update(dt);

        g_timeAccumulator += dt;

        // Cada 4 segundos exactos se recalcula el promedio de CPS
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

    // Método universal de saltos para la versión actual de Geode
    void pushButton(PlayerButton btn, bool isPlayer2) {
        PlayLayer::pushButton(btn, isPlayer2);
        if (!isPlayer2 && btn == PlayerButton::Jump) {
            g_clicksInInterval++;
        }
    }
};

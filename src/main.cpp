#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

int g_clicksInInterval = 0;
cocos2d::CCLabelBMFont* g_cpsLabel = nullptr;
// Usamos el reloj interno de alta precisión para medir el intervalo de 4 segundos
std::chrono::time_point<std::chrono::steady_clock> g_lastResetTime = std::chrono::steady_clock::now();

class $modify(MyPlayLayer, PlayLayer) {
    
    bool init(GJGameLevel* level, bool useReplay, bool dontRunLevel) {
        if (!PlayLayer::init(level, useReplay, dontRunLevel)) return false;

        g_clicksInInterval = 0;
        g_lastResetTime = std::chrono::steady_clock::now();

        g_cpsLabel = CCLabelBMFont::create("CPS: 0.0", "goldFont.fnt");
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        g_cpsLabel->setPosition({ winSize.width - 55.0f, winSize.height - 15.0f });
        g_cpsLabel->setScale(0.55f);
        g_cpsLabel->setOpacity(190);
        
        this->addChild(g_cpsLabel, 999);

        return true;
    }

    // Al salir del nivel, limpiamos el puntero para evitar cierres forzados (crashes)
    void onQuit() {
        PlayLayer::onQuit();
        g_cpsLabel = nullptr;
    }

    // Usamos PlayerInput en lugar del obsoleto PlayerButton para GD 2.2081
    void pushButton(PlayerInput btn, bool isPlayer2) {
        PlayLayer::pushButton(btn, isPlayer2);
        
        // PlayerInput::Jump es el equivalente actual para saltar/cliquear
        if (!isPlayer2 && btn == PlayerInput::Jump) {
            g_clicksInInterval++;
            
            // Calculamos el tiempo transcurrido desde el último reinicio
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<float> elapsed = now - g_lastResetTime;

            // Si ya pasaron los 4 segundos, actualizamos la interfaz y reiniciamos
            if (elapsed.count() >= 4.0f) {
                if (g_cpsLabel) {
                    float cps = static_cast<float>(g_clicksInInterval) / elapsed.count();
                    std::string cpsText = fmt::format("CPS: {:.1f}", cps);
                    g_cpsLabel->setString(cpsText.c_str());
                }
                g_clicksInInterval = 0;
                g_lastResetTime = now;
            }
        }
    }
}

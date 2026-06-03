#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variables globales para el estado del CPS
int g_clicksInInterval = 0;
float g_timeAccumulator = 0.0f; // Acumulador de tiempo en segundos
CCLabelBMFont* g_cpsLabel = nullptr;

class $modify(MyPlayLayer, PlayLayer) {
    
    bool init(GJGameLevel* level, bool useReplay, bool dontRunLevel) {
        if (!PlayLayer::init(level, useReplay, dontRunLevel)) return false;

        // Resetear variables al iniciar o reiniciar el nivel
        g_clicksInInterval = 0;
        g_timeAccumulator = 0.0f;

        // Crear el texto usando la fuente estilizada
        g_cpsLabel = CCLabelBMFont::create("CPS: 0.0", "goldFont.fnt");
        
        // Obtener las dimensiones de la pantalla
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // Posicionar arriba a la derecha
        g_cpsLabel->setPosition({ winSize.width - 55.0f, winSize.height - 15.0f });
        g_cpsLabel->setScale(0.55f);
        g_cpsLabel->setOpacity(190);
        
        this->addChild(g_cpsLabel, 999);

        // Activar la actualización por cuadro (llama a update automáticamente)
        this->scheduleUpdate();

        return true;
    }

    // Registrar las pulsaciones
    void pushButton(PlayerButton button, bool isPlayer2) {
        PlayLayer::pushButton(button, isPlayer2);
        
        if (!isPlayer2 && button == PlayerButton::Jump) {
            g_clicksInInterval++;
        }
    }

    // Se ejecuta en cada cuadro del juego de forma segura
    void update(float dt) {
        PlayLayer::update(dt);

        // Sumar el tiempo transcurrido en este cuadro
        g_timeAccumulator += dt;

        // Cuando pasen los 4 segundos estrictos
        if (g_timeAccumulator >= 4.0f) {
            if (g_cpsLabel) {
                // Calcular CPS usando el tiempo real acumulado para máxima precisión
                float cps = static_cast<float>(g_clicksInInterval) / g_timeAccumulator;
                
                std::string cpsText = fmt::format("CPS: {:.1f}", cps);
                g_cpsLabel->setString(cpsText.c_str());
            }
            
            // Reiniciar contadores para el próximo ciclo
            g_clicksInInterval = 0;
            g_timeAccumulator = 0.0f;
        }
    }
};

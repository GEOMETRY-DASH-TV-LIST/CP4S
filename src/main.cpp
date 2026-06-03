#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variables globales para el estado del CPS
int g_clicksInInterval = 0;
CCLabelBMFont* g_cpsLabel = nullptr;

class $modify(MyPlayLayer, PlayLayer) {
    
    bool init(GJGameLevel* level, bool useReplay, bool dontRunLevel) {
        if (!PlayLayer::init(level, useReplay, dontRunLevel)) return false;

        // Resetear el contador de clicks al iniciar o reiniciar el nivel
        g_clicksInInterval = 0;

        // 1. Crear el texto usando la fuente estilizada de los menús (goldFont)
        g_cpsLabel = CCLabelBMFont::create("CPS: 0.0", "goldFont.fnt");
        
        // Obtener las dimensiones exactas de la pantalla del dispositivo
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // Posicionar el elemento arriba en la esquina derecha de la pantalla (con margen)
        g_cpsLabel->setPosition({ winSize.width - 55.0f, winSize.height - 15.0f });
        
        // Escalarlo adecuadamente para pantallas táctiles de celulares
        g_cpsLabel->setScale(0.55f);
        g_cpsLabel->setOpacity(190); // Sutilmente transparente
        
        // Añadirlo a la capa con máxima prioridad visual (z-order: 999)
        this->addChild(g_cpsLabel, 999);

        // 2. Programar la rutina que se ejecuta estrictamente cada 4 segundos
        this->schedule(schedule_selector(MyPlayLayer::updateCpsInterval), 4.0f);

        return true;
    }

    // Registrar la pulsación táctil (Player 1)
    void pushButton(PlayerButton button, bool isPlayer2) {
        PlayLayer::pushButton(button, isPlayer2);
        
        if (!isPlayer2 && button == PlayerButton::Jump) {
            g_clicksInInterval++;
        }
    }

    // Función ejecutada por el planificador de Cocos2d-x cada 4 segundos
    void updateCpsInterval(float dt) {
        if (g_cpsLabel) {
            // Clicks del lapso divididos entre los 4 segundos transcurridos
            float cps = static_cast<float>(g_clicksInInterval) / 4.0f;
            
            // Re-escribir el texto manteniendo 1 decimal de precisión
            std::string cpsText = fmt::format("CPS: {:.1f}", cps);
            g_cpsLabel->setString(cpsText.c_str());
        }
        
        // REINICIO OBLIGATORIO: Volver a comenzar la cuenta para el siguiente ciclo de 4 segundos
        g_clicksInInterval = 0;
    }
};

#pragma once
#include <cstdint>

namespace Pines {
    // Entradas Analógicas
    constexpr int MICROFONO = 33;     // IO33 - Entrada del senyal del microfòn.
    constexpr int ALTAVOZ = 32;     // IO32 - Sortida cap l'amplificador del senyal d'avís.

    //Control de brillo LDR
    constexpr int LDR = 35;           // IO35 - Entrada LDR pel control de la brillantor.
    // Configuración de la matriz LED
    constexpr int MATRIX_R1 = 25;
    constexpr int MATRIX_G1 = 26;
    constexpr int MATRIX_B1 = 27;
    constexpr int MATRIX_R2 = 14;
    constexpr int MATRIX_G2 = 12;
    constexpr int MATRIX_B2 = 13;
    constexpr int MATRIX_A = 23;
    constexpr int MATRIX_B = 19;
    constexpr int MATRIX_C = 5;
    constexpr int MATRIX_D = 17;
    constexpr int MATRIX_E = 18;
    constexpr int MATRIX_CLK = 16;
    constexpr int MATRIX_LAT = 4;
    constexpr int MATRIX_OE = 15;

     // HDC1080 SENSOR DE TEMPERATURA I HUMITAT
     constexpr uint8_t HDC1080_SCL = 22;
     constexpr uint8_t HDC1080_SDA = 21;

}
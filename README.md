# ESP32 D1 mini and Waveshare e-Paper (EPD) 7.5" V2 (800x480px)

Note: GxEPD2_4G with reset_duration hardcoded to 2ms (this will avoid Busy timeout errors! from the EPD)

Shows info about WiFi connection (you have to create /src/secrets.h) for 20 seconds and afterwards draws some awesome picture in 4 grayscales (Dithered with GIMP and converted with µC/GUI-Bitmap-Converter www.micrium.com)
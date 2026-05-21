
#include "globals.h"


void drawCenteredText(String text, int y, int textSize) {

    int textWidth = text.length() * 6 * textSize;

    int x = (128 - textWidth) / 2;

    display.setTextSize(textSize);
    display.setCursor(x, y);

    display.println(text);
}

void setupDisplay() {

    Wire.begin(D_SDA, D_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED failed");
        while (true);
    }
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.clearDisplay();

    drawCenteredText("Project Sentry", 10, 1);

    drawCenteredText("Initializing...", 40, 1);

    display.display();

    delay(2000); // Show the message for 2 seconds

}void drawCheckmark(int x, int y) {

    // Left stroke
    display.drawLine(x, y, x + 3, y + 3, SSD1306_WHITE);

    // Right stroke
    display.drawLine(x + 3, y + 3, x + 8, y - 3, SSD1306_WHITE);
}

void updateDisplay(int leftCounter, int rightCounter, int automaticState, int syncState, int calibrationState) {

    display.clearDisplay();

    // ===== CIRCLE SETTINGS =====
    int leftCircleX = 36;
    int rightCircleX = 92;

    int circleY = 26;
    int radius = 24;

    // ===== DRAW CIRCLES =====
    display.drawCircle(leftCircleX, circleY, radius, SSD1306_WHITE);
    display.drawCircle(rightCircleX, circleY, radius, SSD1306_WHITE);

    // ===== PERCENT VALUES =====
    int leftPercent = map(leftCounter, 0, left_upper_bound, -100, 100);
    int rightPercent = map(rightCounter, 0, right_upper_bound, -100, 100);

    // ===== TEXT SETTINGS =====
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);

    // ===== LEFT TEXT =====

    display.setCursor(5, 5);
    display.print("L");



    String leftText = String(leftPercent) + "%";

    int leftWidth = leftText.length() * 6 * 2;


    display.print(leftText);

    // ===== RIGHT TEXT =====
    display.setCursor(118, 5);
    display.print("R");
    String rightText = String(rightPercent) + "%";

    int rightWidth = rightText.length() * 6 * 2;

   

    display.print(rightText);

    // ===== LABELS =====
    display.setTextSize(1);

    display.setCursor(leftCircleX - 3, circleY - 20);
    

    display.setCursor(rightCircleX - 3, circleY - 20);
    

    // ===== BOTTOM BAR =====
    

    display.setCursor(0, 56);
    display.print("Auto:");

    if (automaticState) {
        display.print("ON");
    } else {
        display.print("OFF");
    }

    display.setCursor(64, 56);
    display.print("Sync:");

    if (syncState) {
        drawCheckmark(98, 61);
    } else {
        display.print("OFF");
    }

    display.display();
}


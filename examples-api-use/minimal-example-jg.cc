// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)


//first ever LED matrix test using my own code. Draws a line of varying colour moving South East.

#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

using rgb_matrix::Canvas;
using rgb_matrix::RGBMatrix;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo)
{
    interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas)
{
    /*
     * Let's create a simple animation. We use the canvas to draw
     * pixels. We wait between each step to have a slower animation.
     */

    // fill entire canvas blue
    canvas->Fill(0, 0, 255);
    usleep(3 * 1000); // wait 2 seconds

    // clear matrix
    canvas->Clear();

    int x = 0;
    int y = 0;
    int r = 50;
    int g = 50;
    int b = 50;

    // loop to draw a line of 10 pixels
    while (x < 10)
    {
        canvas->SetPixel(x, y, r, g, b);
        usleep(2 * 1000); // wait 2 seconds
        x++;
        y++;
        r = r + 5;
        g = g + 5;
        b = b + 5;
    }
}

int main(int argc, char *argv[])
{
    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "regular"; // or e.g. "adafruit-hat"
    defaults.rows = 32;
    defaults.chain_length = 1;
    defaults.parallel = 1;
    defaults.show_refresh_rate = true;
    Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);
    if (canvas == NULL)
        return 1;

    // It is always good to set up a signal handler to cleanly exit when we
    // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
    // for that.
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    DrawOnCanvas(canvas); // Using the canvas.

    // Animation finished. Shut down the RGB matrix.
    canvas->Clear();
    delete canvas;

    return 0;
}

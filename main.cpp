#include <stdlib.h>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ncurses.h>
#include <signal.h>

// /* The standard ASCII texts people use. */
// const std::string gscale = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

// The one i found on Acerola's vids [https://www.youtube.com/watch?v=gg40RWiaHRY].
const std::string gscale = "▒@?OPoc:. ";

// turn this boolean to true if you are using white theme.
const bool IS_PSYCHO = true;

static void finish(int sig);

int calculateAvg(cv::Mat image, int row, int col, int scale) {
  int sum = 0;
  int pixelCount = 0;
  int r = row, c = col;
  while (r <= row + scale && r < image.rows)
  {
    while (c <= col + scale && c < image.cols)
    {
      sum += (int)image.at<uchar>(r, c);
      pixelCount++;
      c++;
    }
    r++;
  }
  return sum / pixelCount;
}

cv::Mat tempImg;
void preprocessImage(cv::Mat &img, int x, int y) {
  // convert to gray scale image, to get luminance easier.
  cv::cvtColor(img, tempImg, cv::COLOR_BGR2GRAY);
  cv::resize(tempImg, img, cv::Size(x, y));
}

void screen() {
  cv::VideoCapture cam(0);
  int ch, i, j, x, y, index, scale = 4;
  cv::Mat img;
  refresh();
   
  while (true) {
    // Get the screen dimensions.
    getmaxyx(stdscr, y, x);

    // get img camera.
    cam >> img;

    preprocessImage(img, x, y);

    for (i=0; i<y; i++) {
      for (j=0; j<x; j++) {
        if (j <= img.cols && i <= img.rows) {
          // choose text by luminance of image.
          int pixel = calculateAvg(img, i, j, scale);
          index = ceil((gscale.length() - 1) * pixel / 255);

          if (IS_PSYCHO) {
            mvaddch(i, j, gscale[index]);
          } else {
            mvaddch(i, j, gscale[index] | A_REVERSE);
          }
        }
      }
    }
    mvprintw(1, 0, "|%d x %d|", x, y);
    refresh();
  }
  erase();
}

int main(int argc, char *argv[]) {
    (void) signal(SIGINT, finish);
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    screen();
    finish(0);
}

static void finish(int sig) {
  endwin();
  exit(0);
}

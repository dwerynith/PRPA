#include "render.hpp"
#include <cstdint>
#include <cassert>
#include <iostream>
#include <vector>

struct rgb8_t {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
};

rgb8_t heat_lut(float x)
{
  assert(0 <= x && x <= 1);
  float x0 = 1.f / 4.f;
  float x1 = 2.f / 4.f;
  float x2 = 3.f / 4.f;

  if (x < x0)
  {
    auto g = static_cast<std::uint8_t>(x / x0 * 255);
    return rgb8_t{0, g, 255};
  }
  else if (x < x1)
  {
    auto b = static_cast<std::uint8_t>((x1 - x) / x0 * 255);
    return rgb8_t{0, 255, b};
  }
  else if (x < x2)
  {
    auto r = static_cast<std::uint8_t>((x - x1) / x0 * 255);
    return rgb8_t{r, 255, 0};
  }
  else
  {
    auto b = static_cast<std::uint8_t>((1.f - x) / x0 * 255);
    return rgb8_t{255, b, 0};
  }
}


void render(std::byte* buffer,
        int width,
        int height,
        std::ptrdiff_t stride,
        int n_iterations)
{
    std::vector<int> histo = std::vector<int>(n_iterations + 1, 0);
    std::vector<int> array = std::vector<int>(width * height, 0);

    int h2 = height/2;

    for (int y = 0; y < h2; ++y)
    {
        for (int x = 0; x < width; ++x)
        {

            float x0 = (float)x/width * 3.5 - 2.5;
            float y0 = (float)y/height * 2. - 1.;
            float xt = x0;
            float yt = y0;

            int it = 1;

            while (it < n_iterations && xt * xt + yt * yt < 4)
            {
                float xtmp = xt * xt - yt * yt + x0;
                float ytmp = 2. * xt * yt + y0;

                xt = xtmp;
                yt = ytmp;

                ++it;
            }
            histo[it] += 2;
            array[y * width + x] = it;
            array[(height - y - 1) * width + x] = it;
        }
    }

    int total = 0;
    for (int i = 0; i < n_iterations; ++i)
        total += histo[i];

    for (int y = 0; y < height; ++y)
    {
        rgb8_t* lineptr = reinterpret_cast<rgb8_t*>(buffer);

        for (int x = 0; x < width; ++x)
        {
            float h = 0.;
            int it = array[y * width + x];

            for (int i = 0; i <= it; ++i)
                h += (float)histo[i]/(float)total;

            if (h < 1.) {
                lineptr[x] = heat_lut(h);
            } 
            else 
            {
                lineptr[x] = rgb8_t{ 0, 0, 0 };
            }
        }
        buffer += stride;
    }
}


void render_mt(std::byte* buffer,
        int width,
        int height,
        std::ptrdiff_t stride,
        int n_iterations)
{
    render(buffer, width, height, stride, n_iterations);
}

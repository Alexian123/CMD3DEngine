#include "Sprite.h"

using namespace CMD_3D_ENGINE;

Sprite::Sprite(int width, int height)
{   if (width > 0 || height > 0) {
        create(width, height);
    } else {
        create(4, 4);
    }
}

Sprite::Sprite(std::wstring fileName)
{
    if (!load(fileName)) {
        create(4, 4);
    }
}

Sprite::Sprite(const Sprite& sprite)
{
    *this = sprite;
}

Sprite::~Sprite()
{
}

Sprite& Sprite::operator=(const Sprite& right)
{
    create(right.width, right.height);
    for (int i = 0; i < width * height; ++i) {
        this->colors[i] = right.colors[i];
        this->glyphs[i] = right.glyphs[i];
    }
    return *this;
}

int Sprite::getWidth() const
{
    return width;
}

int Sprite::getHeight() const
{
    return height;
}

void Sprite::create(int width, int height)
{
    this->width = width;
    this->height = height;
    colors = std::make_unique<wchar_t[]>(static_cast<size_t>(width) * height);
    glyphs = std::make_unique<wchar_t[]>(static_cast<size_t>(width) * height);
    for (int i = 0; i < width * height; ++i) {
        colors[i] = FG_BLACK;
        glyphs[i] = BLANK_GLYPH;
    }
}

bool Sprite::save(std::wstring fileName)
{
    FILE* f = nullptr;
    _wfopen_s(&f, fileName.c_str(), L"wb");
    if (f == nullptr) {
        return false;
    }
    fwrite(&width, sizeof(int), 1, f);
    fwrite(&height, sizeof(int), 1, f);
    fwrite(colors.get(), sizeof(wchar_t), static_cast<size_t>(width) * height, f);
    fwrite(glyphs.get(), sizeof(wchar_t), static_cast<size_t>(width) * height, f);
    fclose(f);
    return true;
}

bool Sprite::load(std::wstring fileName)
{
    FILE* f = nullptr;
    _wfopen_s(&f, fileName.c_str(), L"rb");
    if (f == nullptr) {
        return false;
    }
    int w = 0, h = 0;
    fread(&w, sizeof(int), 1, f);
    fread(&h, sizeof(int), 1, f);
    create(w, h);
    fread(colors.get(), sizeof(wchar_t), static_cast<size_t>(w) * h, f);
    fread(glyphs.get(), sizeof(wchar_t), static_cast<size_t>(w) * h, f);
    fclose(f);
    return true;
}

void Sprite::setColor(int x, int y, wchar_t color)
{
    if (coordsInBounds(x, y)) {
        colors[static_cast<size_t>(y) * width + x] = color;
    }
}

void Sprite::setGlyph(int x, int y, wchar_t glyph)
{
    if (coordsInBounds(x, y)) {
        glyphs[static_cast<size_t>(y) * width + x] = glyph;
    }
}

wchar_t Sprite::getColor(int x, int y) const
{
    if (coordsInBounds(x, y)) {
        return colors[static_cast<size_t>(y) * width + x];
    }
    return FG_BLACK;
}

wchar_t Sprite::getGlyph(int x, int y) const
{
    if (coordsInBounds(x, y)) {
        return glyphs[static_cast<size_t>(y) * width + x];
    }
    return BLANK_GLYPH;
}

wchar_t Sprite::sampleColor(float x, float y) const
{
    int sx = (int)(x * (float)width);
    int sy = (int)(y * (float)height - 1.0f);
    if (coordsInBounds(sx, sy)) {
        return colors[static_cast<size_t>(sy) * width + sx];
    }
    return FG_BLACK;
}

wchar_t Sprite::sampleGlyph(float x, float y) const
{
    int sx = (int)(x * (float)width);
    int sy = (int)(y * (float)height - 1.0f);
    if (coordsInBounds(sx, sy)) {
        return glyphs[static_cast<size_t>(sy) * width + sx];
    }
    return BLANK_GLYPH;
}

bool Sprite::coordsInBounds(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

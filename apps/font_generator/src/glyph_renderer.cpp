#include "glyph_renderer.hpp"

#include <cassert>
#include <format>


// FT_BitmapGlyph - это указатель
Image to_image(const FT_BitmapGlyph bitmap_glyph, i32 num_components)
{
    if (num_components != 4)
    {
        DV_LOG->write_error(format("to_image(FT_BitmapGlyph): num_components == {}", num_components));
        return Image();
    }

    Image ret(bitmap_glyph->bitmap.width, bitmap_glyph->bitmap.rows, num_components);

    // Если изображение монохромное
    if (bitmap_glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
    {
        // В монохромном изображении один пиксель занимает 1 бит (не байт).
        // pitch - это число байт, занимаемых одной линией изображения
        for (i32 y = 0; y < ret.size().y; ++y)
        {
            u8* src = bitmap_glyph->bitmap.buffer + bitmap_glyph->bitmap.pitch * y;
            u8* dest = ret.data() + y * ret.size().x * 4;

            for (i32 x = 0; x < ret.size().x; ++x)
            {
                dest[x*4 + 0] = 255;
                dest[x*4 + 1] = 255;
                dest[x*4 + 2] = 255;

                // 1) В одном байте умещается 8 пикселей. x >> 3 эквивалентно делению
                //    на 8 (0b1000 превращается в 0b1). Так мы получаем байт, в котором находится пиксель
                // 2) x & 7 - это остаток от деления на 8. Допустим x = 12 = 0b1100
                //    0b1100 & 0b0111 = 0b100 = 4. Так мы получаем номер бита внутри байта.
                // 3) 0x80 == 0b10000000. Единица внутри этого числа сдвигается на номер бита
                //    и побитовой операцией определяется значение бита
                dest[x*4 + 3] = (src[x >> 3] & (0x80 >> (x & 7))) ? 255 : 0;
            }
        }
    }
    else // Grayscale
    {
        // В grayscale изображении каждый пиксель занимает один байт,
        // а значит pitch эквивалентен width
        for (i32 i = 0; i < ret.size().x * ret.size().y; ++i)
        {
            ret.data()[i*4] = 255;
            ret.data()[i*4 + 1] = 255;
            ret.data()[i*4 + 2] = 255;
            ret.data()[i*4 + 3] = bitmap_glyph->bitmap.buffer[i];
        }
    }

    return ret;
}


RenderedGlyph render_glyph_simpe(FT_Face face, const FontSettings& font_settings)
{
    RenderedGlyph ret;

    assert(font_settings.blur_radius >= 0);
#if 0
    //FT_UInt glyphIndex_ = FT_Get_Char_Index(face.get(), 0x2DE0);

    uint32_t glyph_index = FT_Get_Char_Index(face, 41);

    //FT_Set_Pixel_Sizes(face, 0, 48);

    FT_Load_Char(face, 'A', FT_LOAD_DEFAULT);

    FT_UInt glyphIndex_ = 0;

    FT_ULong charCode = FT_Get_First_Char(face, &glyphIndex_);

    for (i32 j = 0; j < 127; ++j)
        charCode = FT_Get_Next_Char(face, charCode, &glyphIndex_);

    FT_Error error1 = FT_Load_Glyph(face, glyphIndex_, FT_LOAD_DEFAULT);

    //FT_Error error1 = FT_Load_Char(face.get(), 'A', FT_LOAD_DEFAULT);

    if (error1)
    {
        DV_LOG->write_error(format("render_glyph_simpe(): FT_Load_Glyph() error | {}", error1));
        return ret;
    }

#endif
    // Реднерим глиф
    FT_Glyph glyph;

    FT_Error error = FT_Get_Glyph(face->glyph, &glyph);

    if (error)
    {
        DV_LOG->write_error(format("render_glyph_simpe(): FT_Get_Glyph() error | {}", error));
        return ret;
    }

    //error = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
    error = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_MONO, nullptr, true);

//    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);

    if (error)
    {
        DV_LOG->write_error(format("render_glyph_simpe(): FT_Glyph_To_Bitmap() error | {}", error));
        FT_Done_Glyph(glyph);
        return ret;
    }

    //FT_Bitmap* ft_bitmap = &face->glyph->bitmap;


    FT_BitmapGlyph bitmap_glyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
    ret.grayscale_image = make_unique<Image>(to_image(bitmap_glyph, 4));
    FT_Done_Glyph(glyph);

    return ret;

    /*
    int blurDistance = Abs(value2_);

    // Загружаем глиф.
    FT_Load_Glyph(face_, glyphIndex_, FT_LOAD_DEFAULT);

    // Метрики можно извлечь и без рендеринга.
    CalculateMetrics();

    // Реднерим глиф.
    FT_Glyph glyph;
    FT_Get_Glyph(face_->glyph, &glyph);
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
    FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
    GlyphManipulator glyphManipulator(bitmapGlyph);
    FT_Done_Glyph(glyph);

    // Размываем, если нужно.
    if (blurDistance > 0)
    {
        glyphManipulator.Blur(blurDistance);

        // Так как размытые глифы предназначены для создания теней, то их центры должны
        // совпадать с центрами неразмытых глифов.
        xOffset_ -= blurDistance;
        yOffset_ -= blurDistance;
    }

    SharedPtr<Image> resultImage = ConvertToImage(glyphManipulator, mainColor_);
    PackGlyph(resultImage);
    */
}

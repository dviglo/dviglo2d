#include "font_generator.hpp"

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>


GeneratedFont generate_font_simple(const FontSettings& font_settings)
{
    GeneratedFont ret;

    FreeTypeFace face(font_settings);

    FT_UInt glyph_index;
    FT_ULong char_code = FT_Get_First_Char(face.get(), &glyph_index);
    vector<RenderedGlyph> rendered_glyphs;
    rendered_glyphs.reserve(face.get()->num_glyphs);
    vector<stbrp_rect> rects;
    rects.reserve(face.get()->num_glyphs);

    i32 vec_ind = 0;

    while (glyph_index != 0)
    {
        FT_Load_Glyph(face.get(), glyph_index, FT_LOAD_TARGET_MONO);
        RenderedGlyph rendered_glyph = render_glyph_simpe(face.get(), font_settings);

        stbrp_rect r{};
        r.id = vec_ind;
        r.w = rendered_glyph.grayscale_image->width();
        r.h = rendered_glyph.grayscale_image->height();
        rects.push_back(r);

        rendered_glyphs.push_back(std::move(rendered_glyph));

        char_code = FT_Get_Next_Char(face.get(), char_code, &glyph_index);

        ++vec_ind;
    }

    stbrp_context pack_context;
    i32 num_nodes = font_settings.texture_size.x;
    vector<stbrp_node> nodes(num_nodes);

    while (true)
    {
        Image current_page(font_settings.texture_size, 4);
        stbrp_init_target(&pack_context, font_settings.texture_size.x, font_settings.texture_size.y, nodes.data(), num_nodes);
        i32 all = stbrp_pack_rects(&pack_context, rects.data(), rects.size());

        for (i32 i = 0; i < rects.size(); ++i)
        {
            stbrp_rect& rect = rects[i];

            if (rect.was_packed)
            {
                current_page.paste(*rendered_glyphs[rect.id].grayscale_image,
                                   ivec2(rect.x, rect.y));
            }
        }

        ret.pages.push_back(current_page);

        if (all)
        {
            break;
        }
        else
        {
            rects.erase(std::remove_if(rects.begin(), rects.end(),
                                       [](const stbrp_rect& rect)
                                       {
                                           return rect.was_packed == true;
                                       }), rects.end());
        }
    }

    return ret;
}

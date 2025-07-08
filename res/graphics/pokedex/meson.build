# Tiles and tilemaps
tiles_png_4bpp = files(
    'scroll_main_background.png',
    'scroll_sub_background.png',
    'dummy1.png',
    'dummy2.png',
    'dummy3.png',
    'entry_main.png',
    'entry_sub.png',
)
# weight_scale.png broken

tiles_png_8bpp = files(
    'scroll_wheel.png',
    'cry_wheel.png',
)

tiles_ncgr_4bpp = ncgr_lz_gen.process(
    tiles_png_4bpp,
    extra_args: [
        '-version101',
        '-sopc',
        '-bitdepth', '4',
        '-nopad',
    ],
)

tiles_ncgr_8bpp = ncgr_lz_gen.process(
    tiles_png_8bpp,
    extra_args: [
        '-version101',
        '-sopc',
        '-bitdepth', '8',
        '-nopad',
    ],
)

tilemaps_nscr = files(
    'scroll_wheel.NSCR',
    'scroll_main_background.NSCR',
    'register_species_window.NSCR',
    'search_local.NSCR',
    'scroll_sub.NSCR',
    'search_national.NSCR',
    'search_filter.NSCR',
    'search_filter_order.NSCR',
    'search_filter_name.NSCR',
    'search_filter_type.NSCR',
    'search_filter_form.NSCR',
    'dummy1.NSCR',
    'dummy2.NSCR',
    'info_main.NSCR',
    'info_species_window.NSCR',
    'info_footprint_window.NSCR',
    'dummy3.NSCR',
    'info_entry_window.NSCR',
    'foreign_entry_window.NSCR',
    'foreign_summary_window.NSCR',
    'banner_local.NSCR',
    'forms_sub.NSCR',
    'page_panel.NSCR',
    'info_sub_unpressed.NSCR',
    'size_sub.NSCR',
    'info_sub.NSCR',
    'info_sub_pressed.NSCR',
    'area_map.NSCR',
    'map_newmoon_island.NSCR',
    'map_seabreak_path.NSCR',
    'map_fullmoon_island.NSCR',
    'map_spring_path.NSCR',
    'area_sub.NSCR',
    'cry_button.NSCR',
    'cry_sub.NSCR',
    'cry_wheel.NSCR',
    'weight_check_main.NSCR',
    'height_check_main.NSCR',
    'search_main.NSCR',
    'weight_scale.NCGR',
)
# weight_scale.NCGR placed here until png is resolved

tilemaps_nscr = lz_gen.process(
    tilemaps_nscr,
    extra_args: [
        '-nopad',
    ],
)

# sprite images
sprites_png_mappingtype0 = files(
    'scroll_buttons.png',
    'page_buttons.png',
    'info_language_buttons.png',
    'size_buttons_pressed.png',
    'time_day_icons.png',
    'cry_dials.png',
    'cursor.png',
    'search_buttons.png',
    'search_button_forms.png',
)

sprites_png_mappingtype128 = files(
    'name_tag.png',
    'search_body_shapes.png',
    'banner.png',
    'type_icons.png',
    'size_check_icons.png',
    'dungeon_highlight.png',
    'unseen_icon.png',
    'dummy4.png',
    'form_display_box.png',
)

sprites_ncgr_mappingtype0 = ncgr_lz_gen.process(
    sprites_png_mappingtype0,
    extra_args: [
        '-version101',
        '-clobbersize',
        '-mappingtype', '0',
        '-nopad',
    ],
)

sprites_ncgr_mappingtype128 = ncgr_lz_gen.process(
    sprites_png_mappingtype128,
    extra_args: [
        '-version101',
        '-clobbersize',
        '-mappingtype', '128',
        '-nopad',
    ],
)

# Sprite cells
sprites_cell_json = files(
    'name_tag_cell.json',
    'search_body_shapes_cell.json',
    'scroll_buttons_cell.json',
    'banner_cell.json',
    'type_icons_cell.json',
    'size_check_icons_cell.json',
    'page_buttons_cell.json',
    'info_language_buttons_cell.json',
    'size_buttons_pressed_cell.json',
    'time_day_icons_cell.json',
    'dungeon_highlight_cell.json',
    'unseen_icon_cell.json',
    'cry_dials_cell.json',
    'dummy4_cell.json',
    'cursor_cell.json',
    'form_display_box_cell.json',
    'search_buttons_cell.json',
    'search_button_forms_cell.json',
)

sprites_ncer = ncer_lz_gen.process(
    sprites_cell_json,
    extra_args: [
        '-nopad',
    ],
)

# Sprite anims
sprites_anim_json = files(
    'name_tag_anim.json',
    'search_body_shapes_anim.json',
    'scroll_buttons_anim.json',
    'banner_anim.json',
    'type_icons_anim.json',
    'size_check_icons_anim.json',
    'page_buttons_anim.json',
    'info_language_buttons_anim.json',
    'size_buttons_pressed_anim.json',
    'time_day_icons_anim.json',
    'dungeon_highlight_anim.json',
    'unseen_icon_anim.json',
    'cry_dials_anim.json',
    'dummy4_anim.json',
    'cursor_anim.json',
    'form_display_box_anim.json',
    'search_buttons_anim.json',
    'search_button_forms_anim.json',
)

sprites_nanr = nanr_lz_gen.process(
    sprites_anim_json,
    extra_args: [
        '-nopad',
    ],
)

# Palettes
palettes_pal = files(
    'background_scroll_default.pal',
    'background_scroll_local.pal',
    'background_scroll_national.pal',
    'buttons.pal',
    'register.pal',
    'search.pal',
    'weight_scale.pal',
    'dummy1.pal',
    'background_sub_1.pal',
    'background_sub_2.pal',
    'background_sub_3.pal',
    'info.pal',
    'banner_default.pal',
    'type_icons.pal',
    'size_check_icons.pal',
    'area_morning.pal',
    'area_day.pal',
    'area_night.pal',
    'cry_dials.pal',
    'dummy4.pal',
    'cry_sound.pal',
    'cursor.pal',
    'form_display_box.pal',
    'banner_local.pal',
    'banner_national.pal',
    'scroll_buttons_national.pal',
    'dummy2.pal',
)

palettes_nclr = nclr_gen.process(palettes_pal,
    extra_args: [
        '-bitdepth', '4',
    ],
)

pokedex_order = files('pokedex.order')
pokedex_graphics_narc = custom_target('zukan.narc',
    output: [
        'zukan.narc',
        'zukan.naix.h',
    ],
    input: [
        tiles_ncgr_4bpp,
        tiles_ncgr_8bpp,
        tilemaps_nscr,
        sprites_ncgr_mappingtype0,
        sprites_ncgr_mappingtype128,
        sprites_ncer,
        sprites_nanr,
        palettes_nclr,
        pokedex_order,
    ],
    command: [
        narc_exe, 'create',
        '--naix',
        '--order', pokedex_order,
        '--output', '@OUTPUT0@',
        '@PRIVATE_DIR@',
    ]
)

nitrofs_files += pokedex_graphics_narc
naix_headers += pokedex_graphics_narc[1]

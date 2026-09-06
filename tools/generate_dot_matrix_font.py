#!/usr/bin/env python3
import math
import struct

def create_dot_matrix_ttf(output_path):
    patterns = {
        '0': [" 000 ", "0   0", "0  00", "0 0 0", "00  0", "0   0", " 000 "],
        '1': ["  0  ", " 00  ", "  0  ", "  0  ", "  0  ", "  0  ", " 000 "],
        '2': [" 000 ", "0   0", "    0", "   0 ", "  0  ", " 0   ", "00000"],
        '3': [" 000 ", "0   0", "    0", "  00 ", "    0", "0   0", " 000 "],
        '4': ["   0 ", "  00 ", " 0 0 ", "0  0 ", "00000", "   0 ", "   0 "],
        '5': ["00000", "0    ", "0000 ", "    0", "    0", "0   0", " 000 "],
        '6': [" 000 ", "0   0", "0    ", "0000 ", "0   0", "0   0", " 000 "],
        '7': ["00000", "    0", "   0 ", "  0  ", " 0   ", " 0   ", " 0   "],
        '8': [" 000 ", "0   0", "0   0", " 000 ", "0   0", "0   0", " 000 "],
        '9': [" 000 ", "0   0", "0   0", " 0000", "    0", "0   0", " 000 "],
        ':': ["     ", "  0  ", "  0  ", "     ", "  0  ", "  0  ", "     "],
        ',': ["     ", "     ", "     ", "     ", "  0  ", "  0  ", " 0   "],
        '.': ["     ", "     ", "     ", "     ", "     ", "  0  ", "  0  "],
        '+': ["     ", "  0  ", "  0  ", "00000", "  0  ", "  0  ", "     "],
        '-': ["     ", "     ", "     ", "00000", "     ", "     ", "     "],
        '|': ["  0  ", "  0  ", "  0  ", "  0  ", "  0  ", "  0  ", "  0  "],
        ' ': ["     ", "     ", "     ", "     ", "     ", "     ", "     "]
    }

    units_per_em = 1000
    dot_size = 100
    spacing = 140
    margin_x = 100

    glyph_chars = ['.notdef', ' '] + sorted([c for c in patterns if c != ' '])
    char_to_gid = {c: i for i, c in enumerate(glyph_chars)}

    def make_dot_contour(x0, y0, sz):
        r = sz // 2
        cx = x0 + r
        cy = y0 + r
        return [
            (cx - r, cy - r//2, True),
            (cx - r//2, cy - r, True),
            (cx + r//2, cy - r, True),
            (cx + r, cy - r//2, True),
            (cx + r, cy + r//2, True),
            (cx + r//2, cy + r, True),
            (cx - r//2, cy + r, True),
            (cx - r, cy + r//2, True),
        ]

    glyf_data = bytearray()
    loca_offsets = [0]
    hmtx_data = bytearray()

    for gid, char in enumerate(glyph_chars):
        if char in ('.notdef', ' '):
            if len(glyf_data) % 2 != 0:
                glyf_data.append(0)
            loca_offsets.append(len(glyf_data))
            hmtx_data += struct.pack('>hh', 600, 0)
            continue

        pattern = patterns.get(char, patterns[' '])
        contours = []
        for row_idx, row in enumerate(pattern):
            y = 700 - (row_idx * spacing)
            for col_idx, ch in enumerate(row):
                if ch != ' ':
                    x = margin_x + (col_idx * spacing)
                    contours.append(make_dot_contour(x, y, dot_size))

        if not contours:
            if len(glyf_data) % 2 != 0:
                glyf_data.append(0)
            loca_offsets.append(len(glyf_data))
            hmtx_data += struct.pack('>hh', 800, 0)
            continue

        num_contours = len(contours)
        all_pts = []
        end_pts = []
        pt_count = 0

        for c in contours:
            all_pts.extend(c)
            pt_count += len(c)
            end_pts.append(pt_count - 1)

        xmin = min(p[0] for p in all_pts)
        ymin = min(p[1] for p in all_pts)
        xmax = max(p[0] for p in all_pts)
        ymax = max(p[1] for p in all_pts)

        g_bytes = bytearray(struct.pack('>hhhhh', num_contours, xmin, ymin, xmax, ymax))
        for ep in end_pts: g_bytes += struct.pack('>H', ep)
        g_bytes += struct.pack('>H', 0) # instructions

        flags = bytearray()
        x_coords = bytearray()
        y_coords = bytearray()

        last_x = 0
        last_y = 0

        for px, py, on_curve in all_pts:
            dx = px - last_x
            dy = py - last_y
            last_x = px
            last_y = py

            flag = 0x01 if on_curve else 0

            if dx == 0:
                flag |= 0x10
            elif 0 <= dx <= 255:
                flag |= 0x02 | 0x10
                x_coords.append(dx)
            elif -255 <= dx < 0:
                flag |= 0x02
                x_coords.append(-dx)
            else:
                x_coords += struct.pack('>h', dx)

            if dy == 0:
                flag |= 0x20
            elif 0 <= dy <= 255:
                flag |= 0x04 | 0x20
                y_coords.append(dy)
            elif -255 <= dy < 0:
                flag |= 0x04
                y_coords.append(-dy)
            else:
                y_coords += struct.pack('>h', dy)

            flags.append(flag)

        g_bytes += flags + x_coords + y_coords
        if len(g_bytes) % 2 != 0:
            g_bytes.append(0)

        glyf_data += g_bytes
        loca_offsets.append(len(glyf_data))
        hmtx_data += struct.pack('>hh', 850, xmin)

    loca_data = bytearray()
    for off in loca_offsets:
        loca_data += struct.pack('>H', off // 2)

    pairs = sorted([(ord(c), i) for i, c in enumerate(glyph_chars) if c != '.notdef'])
    runs = []
    curr_start_u, curr_end_u, curr_start_g, curr_end_g = pairs[0][0], pairs[0][0], pairs[0][1], pairs[0][1]
    for u, g in pairs[1:]:
        if u == curr_end_u + 1 and g == curr_end_g + 1:
            curr_end_u = u
            curr_end_g = g
        else:
            runs.append((curr_start_u, curr_end_u, curr_start_g))
            curr_start_u, curr_end_u, curr_start_g, curr_end_g = u, u, g, g
    runs.append((curr_start_u, curr_end_u, curr_start_g))

    end_count = [r[1] for r in runs] + [0xFFFF]
    start_count = [r[0] for r in runs] + [0xFFFF]
    id_delta = [(r[2] - r[0]) & 0xFFFF for r in runs] + [1]
    id_range_offset = [0] * len(start_count)

    seg_count = len(start_count)
    search_range = 2 * (2 ** int(math.log2(seg_count)))
    entry_selector = int(math.log2(seg_count))
    range_shift = (seg_count * 2) - search_range

    subtable = bytearray(struct.pack('>HHH', 4, 16 + 8 * seg_count, 0))
    subtable += struct.pack('>HHHH', seg_count * 2, search_range, entry_selector, range_shift)
    for ec in end_count: subtable += struct.pack('>H', ec)
    subtable += struct.pack('>H', 0)
    for sc in start_count: subtable += struct.pack('>H', sc)
    for delta in id_delta: subtable += struct.pack('>H', delta)
    for ro in id_range_offset: subtable += struct.pack('>H', ro)

    cmap_data = struct.pack('>HH', 0, 2)
    cmap_data += struct.pack('>HHI', 0, 3, 20)
    cmap_data += struct.pack('>HHI', 3, 1, 20)
    cmap_data += subtable

    names = {
        1: "Matrix Dot",
        2: "Regular",
        3: "1.000;MatrixDot;MatrixDot-Regular",
        4: "Matrix Dot",
        6: "MatrixDot-Regular"
    }

    name_records = bytearray()
    string_data = bytearray()

    for name_id, text in names.items():
        encoded = text.encode('utf-16be')
        name_records += struct.pack('>HHHHHH', 3, 1, 0x0409, name_id, len(encoded), len(string_data))
        string_data += encoded

    name_table_header = struct.pack('>HHH', 0, len(names), 6 + len(names) * 12)
    name_data = name_table_header + name_records + string_data

    maxp_data = struct.pack('>IHHHHHHHHHHHHH', 0x00010000, len(glyph_chars), 64, 64, 16, 0, 0, 2, 0, 0, 0, 0, 0, 0)

    # 17 fields for head
    head_data = struct.pack('>IIIIHHqqhhhhHHhhh',
                            0x00010000, 0x00010000, 0, 0x5F0F3CF5,
                            0, units_per_em,
                            0, 0,
                            0, 0, 900, 700,
                            0, 8, 2, 0, 0)

    hhea_data = struct.pack('>Ihhhhhhh hhhhhhhh H'.replace(' ', ''),
                            0x00010000, 800, -200, 0,
                            850, 0, 0, 850,
                            1, 0, 0,
                            0, 0, 0, 0,
                            0, len(glyph_chars))

    os2_data = struct.pack('>H hHHH hhhhhh hhhhh 10s IIII 4s HHH hhh HH'.replace(' ', ''),
                           3, 400, 400, 5, 0,
                           0, 0, 0, 0, 0, 0,
                           0, 0, 0, 0, 0,
                           b'\x00'*10,
                           1, 0, 0, 0,
                           b'MATR',
                           64, 32, 126,
                           800, -200, 0,
                           800, 200)

    post_data = struct.pack('>IihhIIIII', 0x00030000, 0, 0, 0, 1, 0, 0, 0, 0)

    def align4(b):
        while len(b) % 4 != 0:
            b.append(0)
        return b

    tables = {
        b'OS/2': align4(bytearray(os2_data)),
        b'cmap': align4(bytearray(cmap_data)),
        b'glyf': align4(bytearray(glyf_data)),
        b'head': align4(bytearray(head_data)),
        b'hhea': align4(bytearray(hhea_data)),
        b'hmtx': align4(bytearray(hmtx_data)),
        b'loca': align4(bytearray(loca_data)),
        b'maxp': align4(bytearray(maxp_data)),
        b'name': align4(bytearray(name_data)),
        b'post': align4(bytearray(post_data))
    }

    num_tables = len(tables)
    header = struct.pack('>IHHHH', 0x00010000, num_tables, 128, 3, 32)
    table_dir = bytearray()
    offset = 12 + num_tables * 16

    for tag in sorted(tables.keys()):
        data = tables[tag]
        sum32 = 0
        for i in range(0, len(data), 4):
            sum32 = (sum32 + struct.unpack('>I', data[i:i+4])[0]) & 0xFFFFFFFF
        table_dir += struct.pack('>4sIII', tag, sum32, offset, len(data))
        offset += len(data)

    font_bytes = bytearray(header + table_dir)
    for tag in sorted(tables.keys()):
        font_bytes += tables[tag]

    with open(output_path, 'wb') as f:
        f.write(font_bytes)

    print(f"Generated Matrix Dot TrueType font successfully at {output_path}")

if __name__ == '__main__':
    create_dot_matrix_ttf('/home/devansh/Documents/Arduor-Plus/gtk2_ardour/MatrixDot.ttf')

#!/usr/bin/env python3
"""Genererer illustrerte bildekort (J, Q, K) for alle 4 farger."""

from PIL import Image, ImageDraw, ImageFont
import os

CARD_W, CARD_H = 500, 700
BORDER_RADIUS = 20
OUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "Kort")

# ─── Farger ───────────────────────────────────────────────────────────────────
WHITE      = (255, 255, 255)
BLACK      = (20, 20, 20)
RED        = (220, 20, 20)
BORDER_COL = (180, 180, 180)
BLUE       = (20, 50, 150)
DARK_BLUE  = (10, 30, 100)
RED_ROBE   = (200, 30, 30)
DARK_RED   = (150, 20, 20)
GOLD       = (240, 200, 50)
DARK_GOLD  = (190, 155, 30)
SKIN       = (255, 218, 185)
DARK_SKIN  = (220, 180, 145)
HAIR_BRN   = (80, 50, 20)
HAIR_YLW   = (210, 175, 50)
CREAM      = (255, 250, 235)

SUITS = {
    "S": {"sym": "\u2660", "color": BLACK},
    "H": {"sym": "\u2665", "color": RED},
    "D": {"sym": "\u2666", "color": RED},
    "C": {"sym": "\u2663", "color": BLACK},
}

# ─── Fonter ───────────────────────────────────────────────────────────────────
def load_font(size):
    for p in ['/System/Library/Fonts/Helvetica.ttc', '/Library/Fonts/Arial.ttf']:
        try: return ImageFont.truetype(p, size)
        except: pass
    return ImageFont.load_default()

def load_sym(size):
    for p in ['/System/Library/Fonts/Supplemental/Apple Symbols.ttf',
              '/System/Library/Fonts/Apple Symbols.ttf',
              '/System/Library/Fonts/Helvetica.ttc']:
        try: return ImageFont.truetype(p, size)
        except: pass
    return ImageFont.load_default()

font_rank  = load_font(64)
font_suit  = load_sym(48)
font_frame = load_sym(36)

def text_center(draw, pos, text, font, fill):
    bbox = draw.textbbox((0, 0), text, font=font)
    tw, th = bbox[2] - bbox[0], bbox[3] - bbox[1]
    draw.text((pos[0] - tw // 2, pos[1] - th // 2), text, font=font, fill=fill)

def draw_corners(img, draw, rank, sym, color):
    draw.text((18, 18), rank, font=font_rank, fill=color)
    draw.text((20, 80), sym, font=font_suit, fill=color)
    tmp = Image.new('RGBA', (80, 110), (0, 0, 0, 0))
    td = ImageDraw.Draw(tmp)
    td.text((8, 0), rank, font=font_rank, fill=color)
    td.text((10, 58), sym, font=font_suit, fill=color)
    tmp = tmp.rotate(180, expand=False)
    img.paste(tmp, (CARD_W - 90, CARD_H - 120), tmp)

# ─── Figur-tegning ────────────────────────────────────────────────────────────

def draw_king(fd, w, h):
    cx = w // 2

    # ── Kropp / kappe (bakgrunn først) ──
    body_top = 95
    # Venstre halvdel: blå
    fd.polygon([(0, body_top), (cx, body_top), (cx, h), (0, h)], fill=BLUE)
    # Høyre halvdel: rød
    fd.polygon([(cx, body_top), (w, body_top), (w, h), (cx, h)], fill=RED_ROBE)
    # Gull-kanter
    fd.rectangle((0, body_top, 14, h), fill=GOLD)
    fd.rectangle((w - 14, body_top, w, h), fill=GOLD)
    fd.rectangle((cx - 6, body_top, cx + 6, h), fill=GOLD, outline=DARK_GOLD)

    # Belte
    by = body_top + 55
    fd.rectangle((0, by, w, by + 14), fill=GOLD, outline=DARK_GOLD)
    fd.rectangle((cx - 12, by - 3, cx + 12, by + 17), fill=GOLD, outline=DARK_GOLD)
    fd.rectangle((cx - 7, by + 2, cx + 7, by + 12), fill=BLUE)

    # Ermer med gull-mansjetter
    fd.ellipse((5, body_top + 10, 55, body_top + 45), fill=BLUE, outline=DARK_BLUE)
    fd.ellipse((w - 55, body_top + 10, w - 5, body_top + 45), fill=RED_ROBE, outline=DARK_RED)

    # ── Sverd (bak kropp, håndtak synlig) ──
    sx = cx + 42
    fd.rectangle((sx - 3, 15, sx + 3, body_top + 30), fill=(170, 170, 180), outline=(130, 130, 140))
    fd.rectangle((sx - 16, 50, sx + 16, 57), fill=GOLD, outline=DARK_GOLD)
    fd.ellipse((sx - 7, 35, sx + 7, 50), fill=GOLD, outline=DARK_GOLD)

    # ── Krone ──
    cr_y = 3
    cr_pts = [
        (cx - 38, 42), (cx - 38, cr_y + 18),
        (cx - 28, cr_y + 8), (cx - 18, cr_y + 18),
        (cx - 8, cr_y), (cx + 8, cr_y),
        (cx + 18, cr_y + 18), (cx + 28, cr_y + 8),
        (cx + 38, cr_y + 18), (cx + 38, 42),
    ]
    fd.polygon(cr_pts, fill=GOLD, outline=DARK_GOLD)
    fd.rectangle((cx - 38, 36, cx + 38, 42), fill=DARK_GOLD)
    for jx in [cx - 28, cx, cx + 28]:
        fd.ellipse((jx - 5, cr_y + 6, jx + 5, cr_y + 16), fill=RED_ROBE)

    # ── Ansikt ──
    fy = 42
    fd.ellipse((cx - 32, fy, cx + 32, fy + 55), fill=SKIN, outline=DARK_SKIN)
    # Øyne
    ey = fy + 18
    for ex in [cx - 14, cx + 6]:
        fd.ellipse((ex, ey, ex + 8, ey + 8), fill=WHITE)
        fd.ellipse((ex + 2, ey + 2, ex + 6, ey + 6), fill=BLACK)
    # Nese
    fd.polygon([(cx, ey + 10), (cx - 4, ey + 20), (cx + 4, ey + 20)], fill=DARK_SKIN)
    # Munn
    fd.arc((cx - 7, ey + 22, cx + 7, ey + 30), 0, 180, fill=(180, 80, 80), width=2)

    # ── Skjegg ──
    beard = [
        (cx - 28, fy + 38), (cx - 30, fy + 55), (cx - 22, fy + 68),
        (cx - 10, fy + 73), (cx, fy + 76), (cx + 10, fy + 73),
        (cx + 22, fy + 68), (cx + 30, fy + 55), (cx + 28, fy + 38),
    ]
    fd.polygon(beard, fill=HAIR_BRN, outline=(60, 35, 10))

    # Hvit krage
    coly = fy + 52
    fd.polygon([
        (cx - 42, coly + 8), (cx - 15, coly - 5), (cx, coly + 5),
        (cx + 15, coly - 5), (cx + 42, coly + 8),
        (cx + 32, coly + 22), (cx - 32, coly + 22),
    ], fill=WHITE, outline=(200, 200, 200))

    # ── Hender ──
    fd.ellipse((10, by + 16, 48, by + 42), fill=SKIN, outline=DARK_SKIN)
    fd.ellipse((sx - 14, 58, sx + 10, 82), fill=SKIN, outline=DARK_SKIN)

    # Dekorative detaljer på kappen
    for dy in range(by + 20, h, 30):
        fd.rectangle((18, dy, 40, dy + 6), fill=GOLD)
        fd.rectangle((w - 40, dy, w - 18, dy + 6), fill=GOLD)


def draw_queen(fd, w, h):
    cx = w // 2

    # ── Kjole (bakgrunn) ──
    body_top = 100

    # Diagonal deling: rød og blå
    fd.polygon([(0, body_top), (w, body_top), (w, h), (0, h)], fill=RED_ROBE)
    fd.polygon([(0, body_top), (w, h), (0, h)], fill=BLUE)

    # Diagonal gullstripe
    for offset in range(-8, 9, 4):
        fd.line([(cx - 70 + offset, body_top), (cx + 70 + offset, h)], fill=GOLD, width=3)

    # Gullkanter
    fd.rectangle((0, body_top, 12, h), fill=GOLD)
    fd.rectangle((w - 12, body_top, w, h), fill=GOLD)
    fd.rectangle((0, body_top, w, body_top + 5), fill=GOLD)

    # Dekorative border-mønster
    for dy in range(body_top + 20, h, 25):
        fd.rectangle((15, dy, 30, dy + 5), fill=GOLD)
        fd.rectangle((w - 30, dy, w - 15, dy + 5), fill=GOLD)

    # ── Hår (bak hodet) ──
    fd.ellipse((cx - 42, 28, cx + 42, 100), fill=HAIR_YLW)
    fd.ellipse((cx - 48, 55, cx - 22, 110), fill=HAIR_YLW)
    fd.ellipse((cx + 22, 55, cx + 48, 110), fill=HAIR_YLW)

    # ── Krone ──
    cr_y = 5
    cr_pts = [
        (cx - 30, 38), (cx - 30, cr_y + 12),
        (cx - 22, cr_y + 5), (cx - 14, cr_y + 14),
        (cx - 5, cr_y), (cx + 5, cr_y),
        (cx + 14, cr_y + 14), (cx + 22, cr_y + 5),
        (cx + 30, cr_y + 12), (cx + 30, 38),
    ]
    fd.polygon(cr_pts, fill=GOLD, outline=DARK_GOLD)
    fd.rectangle((cx - 30, 32, cx + 30, 38), fill=DARK_GOLD)
    for jx in [cx - 22, cx, cx + 22]:
        fd.ellipse((jx - 4, cr_y + 5, jx + 4, cr_y + 13), fill=(0, 150, 50))

    # ── Ansikt ──
    fy = 38
    fd.ellipse((cx - 28, fy, cx + 28, fy + 50), fill=SKIN, outline=DARK_SKIN)
    # Øyne (blå)
    ey = fy + 16
    for ex in [cx - 13, cx + 5]:
        fd.ellipse((ex, ey, ex + 8, ey + 7), fill=WHITE)
        fd.ellipse((ex + 2, ey + 1, ex + 6, ey + 5), fill=BLUE)
    # Nese
    fd.polygon([(cx, ey + 8), (cx - 3, ey + 16), (cx + 3, ey + 16)], fill=DARK_SKIN)
    # Munn (røde lepper)
    fd.arc((cx - 6, ey + 18, cx + 6, ey + 24), 0, 180, fill=RED, width=2)
    fd.ellipse((cx - 5, ey + 18, cx + 5, ey + 22), fill=(220, 80, 80))

    # Hals
    fd.rectangle((cx - 8, fy + 48, cx + 8, fy + 62), fill=SKIN)
    # Halskjede
    fd.arc((cx - 12, fy + 50, cx + 12, fy + 65), 0, 180, fill=GOLD, width=2)
    fd.ellipse((cx - 4, fy + 60, cx + 4, fy + 68), fill=RED_ROBE)

    # Hvit krage / utringning
    coly = fy + 60
    fd.polygon([
        (cx - 48, coly + 10), (cx - 20, coly - 5),
        (cx, coly + 8), (cx + 20, coly - 5),
        (cx + 48, coly + 10), (cx + 35, coly + 22), (cx - 35, coly + 22),
    ], fill=WHITE, outline=(200, 200, 200))

    # ── Blomst i hånden ──
    fl_x = cx - 40
    fl_y = body_top + 40
    fd.line([(fl_x, fl_y - 5), (fl_x, fl_y + 45)], fill=(0, 130, 0), width=3)
    # Blader
    fd.polygon([(fl_x, fl_y + 20), (fl_x - 12, fl_y + 30), (fl_x, fl_y + 35)], fill=(0, 150, 0))
    fd.polygon([(fl_x, fl_y + 25), (fl_x + 12, fl_y + 35), (fl_x, fl_y + 40)], fill=(0, 150, 0))
    # Kronblader
    for dx, dy in [(-8, 0), (8, 0), (0, -8), (0, 8), (-6, -6), (6, -6), (-6, 6), (6, 6)]:
        fd.ellipse((fl_x + dx - 5, fl_y + dy - 5, fl_x + dx + 5, fl_y + dy + 5), fill=GOLD)
    fd.ellipse((fl_x - 4, fl_y - 4, fl_x + 4, fl_y + 4), fill=RED_ROBE)

    # Hender
    fd.ellipse((fl_x - 12, fl_y + 12, fl_x + 10, fl_y + 35), fill=SKIN, outline=DARK_SKIN)
    fd.ellipse((cx + 25, body_top + 30, cx + 50, body_top + 55), fill=SKIN, outline=DARK_SKIN)


def draw_jack(fd, w, h):
    cx = w // 2

    # ── Kropp ──
    body_top = 92
    # Venstre rød, høyre blå
    fd.polygon([(0, body_top), (cx, body_top), (cx, h), (0, h)], fill=RED_ROBE)
    fd.polygon([(cx, body_top), (w, body_top), (w, h), (cx, h)], fill=BLUE)
    # Gull-sentrum
    fd.rectangle((cx - 5, body_top, cx + 5, h), fill=GOLD, outline=DARK_GOLD)
    # Gull-kanter
    fd.rectangle((0, body_top, 10, h), fill=GOLD)
    fd.rectangle((w - 10, body_top, w, h), fill=GOLD)
    fd.rectangle((0, body_top, w, body_top + 4), fill=GOLD)

    # Belte
    by = body_top + 50
    fd.rectangle((0, by, w, by + 12), fill=GOLD, outline=DARK_GOLD)
    fd.rectangle((cx - 10, by - 2, cx + 10, by + 14), fill=GOLD, outline=DARK_GOLD)

    # Ermer
    fd.ellipse((5, body_top + 8, 50, body_top + 40), fill=RED_ROBE, outline=DARK_RED)
    fd.ellipse((w - 50, body_top + 8, w - 5, body_top + 40), fill=BLUE, outline=DARK_BLUE)

    # Dekor
    for dy in range(by + 18, h, 25):
        fd.rectangle((15, dy, 32, dy + 5), fill=GOLD)
        fd.rectangle((w - 32, dy, w - 15, dy + 5), fill=GOLD)

    # ── Lue / barett ──
    ht_y = 3
    fd.ellipse((cx - 38, ht_y, cx + 38, ht_y + 32), fill=RED_ROBE, outline=DARK_RED)
    fd.rectangle((cx - 40, ht_y + 20, cx + 40, ht_y + 28), fill=DARK_RED)
    # Fjær
    fd.polygon([
        (cx + 18, ht_y + 6), (cx + 55, ht_y - 12),
        (cx + 52, ht_y + 4), (cx + 20, ht_y + 12),
    ], fill=BLUE)
    fd.line([(cx + 18, ht_y + 8), (cx + 54, ht_y - 6)], fill=DARK_GOLD, width=1)
    # Juvel på luen
    fd.ellipse((cx - 6, ht_y + 10, cx + 6, ht_y + 22), fill=GOLD, outline=DARK_GOLD)

    # ── Hår ──
    fd.ellipse((cx - 34, ht_y + 22, cx - 18, ht_y + 60), fill=HAIR_YLW)
    fd.ellipse((cx + 18, ht_y + 22, cx + 34, ht_y + 60), fill=HAIR_YLW)

    # ── Ansikt ──
    fy = ht_y + 28
    fd.ellipse((cx - 28, fy, cx + 28, fy + 48), fill=SKIN, outline=DARK_SKIN)
    # Øyne
    ey = fy + 14
    for ex in [cx - 13, cx + 5]:
        fd.ellipse((ex, ey, ex + 8, ey + 7), fill=WHITE)
        fd.ellipse((ex + 2, ey + 2, ex + 6, ey + 5), fill=BLACK)
    # Nese
    fd.polygon([(cx, ey + 8), (cx - 3, ey + 16), (cx + 3, ey + 16)], fill=DARK_SKIN)
    # Munn
    fd.arc((cx - 5, ey + 18, cx + 5, ey + 24), 0, 180, fill=(180, 80, 80), width=2)
    # Bart
    fd.arc((cx - 10, ey + 13, cx - 1, ey + 20), 200, 340, fill=HAIR_BRN, width=2)
    fd.arc((cx + 1, ey + 13, cx + 10, ey + 20), 200, 340, fill=HAIR_BRN, width=2)

    # Hvit krage
    coly = fy + 45
    fd.polygon([
        (cx - 44, coly + 8), (cx - 14, coly - 6),
        (cx, coly + 6), (cx + 14, coly - 6),
        (cx + 44, coly + 8), (cx + 34, coly + 22), (cx - 34, coly + 22),
    ], fill=WHITE, outline=(200, 200, 200))

    # ── Hellebard / Stav ──
    sx = cx + 46
    fd.rectangle((sx - 3, 10, sx + 3, body_top + 50), fill=HAIR_BRN, outline=(60, 35, 10))
    # Øksehode
    fd.polygon([
        (sx + 3, 18), (sx + 24, 10), (sx + 28, 25),
        (sx + 24, 40), (sx + 3, 42),
    ], fill=(170, 170, 185), outline=(120, 120, 130))
    fd.polygon([(sx + 3, 10), (sx + 8, 5), (sx + 3, 18)], fill=(170, 170, 185), outline=(120, 120, 130))

    # Hender
    fd.ellipse((sx - 14, by - 8, sx + 10, by + 16), fill=SKIN, outline=DARK_SKIN)
    fd.ellipse((8, by + 14, 46, by + 40), fill=SKIN, outline=DARK_SKIN)


# ─── Lag et komplett bildekort ─────────────────────────────────────────────────

def create_face_card(rank, suit_key):
    suit = SUITS[suit_key]
    color = suit["color"]
    sym = suit["sym"]

    img = Image.new('RGB', (CARD_W, CARD_H), WHITE)
    draw = ImageDraw.Draw(img)
    draw.rounded_rectangle((0, 0, CARD_W - 1, CARD_H - 1), radius=BORDER_RADIUS,
                           fill=WHITE, outline=BORDER_COL, width=3)

    # Ramme-dimensjoner
    fx0, fy0 = 65, 108
    fx1, fy1 = CARD_W - 65, CARD_H - 108
    fw = fx1 - fx0
    fh = fy1 - fy0
    half_h = fh // 2

    # ── Tegn øvre halvdel av figuren ──
    fig_top = Image.new('RGB', (fw, half_h), CREAM)
    fd = ImageDraw.Draw(fig_top)

    if rank == 'K':
        draw_king(fd, fw, half_h)
    elif rank == 'Q':
        draw_queen(fd, fw, half_h)
    elif rank == 'J':
        draw_jack(fd, fw, half_h)

    # Lim inn øvre halvdel
    img.paste(fig_top, (fx0, fy0))

    # Roter 180° og lim inn nedre halvdel
    fig_bot = fig_top.rotate(180)
    img.paste(fig_bot, (fx0, fy0 + half_h))

    # ── Tegn ramme og dekor oppå ──
    draw = ImageDraw.Draw(img)

    # Blå ramme
    draw.rectangle((fx0, fy0, fx1, fy1), fill=None, outline=BLUE, width=4)

    # Gull midtlinje
    mid_y = fy0 + half_h
    draw.line([(fx0 + 4, mid_y), (fx1 - 4, mid_y)], fill=GOLD, width=3)
    draw.line([(fx0 + 4, mid_y - 2), (fx1 - 4, mid_y - 2)], fill=DARK_GOLD, width=1)
    draw.line([(fx0 + 4, mid_y + 2), (fx1 - 4, mid_y + 2)], fill=DARK_GOLD, width=1)

    # Suit-symboler i rammens hjørner
    text_center(draw, (fx0 + 22, fy0 + 22), sym, font_frame, color)
    text_center(draw, (fx1 - 22, fy0 + 22), sym, font_frame, color)
    text_center(draw, (fx0 + 22, fy1 - 22), sym, font_frame, color)
    text_center(draw, (fx1 - 22, fy1 - 22), sym, font_frame, color)

    # ── Hjørner med rang og suit ──
    draw_corners(img, draw, rank, sym, color)

    return img


# ─── Hovedprogram ─────────────────────────────────────────────────────────────
def main():
    count = 0
    for suit_key in ["S", "H", "D", "C"]:
        for rank in ["J", "Q", "K"]:
            # Slett gammel fil
            fn = f"{suit_key}{rank}.jpg"
            path = os.path.join(OUT_DIR, fn)
            if os.path.exists(path):
                os.remove(path)

            img = create_face_card(rank, suit_key)
            img.save(path, "JPEG", quality=95)
            count += 1
            print(f"  [{count:2d}/12] {rank} {SUITS[suit_key]['sym']} -> {fn}")

    print(f"\nFerdig! {count} bildekort oppdatert i {OUT_DIR}")


if __name__ == "__main__":
    main()

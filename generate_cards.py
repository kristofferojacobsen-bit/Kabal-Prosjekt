#!/usr/bin/env python3
"""Genererer 53 spillkort-bilder (52 kort + 1 bakside) i JPG-format."""

from PIL import Image, ImageDraw, ImageFont
import os

CARD_W, CARD_H = 500, 700
BORDER_RADIUS = 20
OUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "cards")
os.makedirs(OUT_DIR, exist_ok=True)

WHITE = (255, 255, 255)
BLACK = (20, 20, 20)
RED   = (220, 20, 20)
BORDER_COL = (180, 180, 180)

SUITS = {
    "S": {"sym": "\u2660", "color": BLACK},
    "H": {"sym": "\u2665", "color": RED},
    "D": {"sym": "\u2666", "color": RED},
    "C": {"sym": "\u2663", "color": BLACK},
}
RANKS = ["A","2","3","4","5","6","7","8","9","10","J","Q","K"]

# ─── Fonter ───────────────────────────────────────────────────────────────────
def load_font(size):
    for p in ['/System/Library/Fonts/Helvetica.ttc', '/Library/Fonts/Arial.ttf']:
        try: return ImageFont.truetype(p, size)
        except: pass
    return ImageFont.load_default()

def load_sym(size):
    for p in ['/System/Library/Fonts/Supplemental/Apple Symbols.ttf',
              '/System/Library/Fonts/Apple Symbols.ttf',
              '/Library/Fonts/Arial Unicode.ttf',
              '/System/Library/Fonts/Helvetica.ttc']:
        try: return ImageFont.truetype(p, size)
        except: pass
    return ImageFont.load_default()

font_rank    = load_font(64)
font_suit_s  = load_sym(48)
font_pip     = load_sym(56)
font_big_ace = load_sym(200)
font_face_l  = load_font(180)
font_face_sym = load_sym(120)

def text_center(draw, pos, text, font, fill):
    bbox = draw.textbbox((0, 0), text, font=font)
    tw, th = bbox[2] - bbox[0], bbox[3] - bbox[1]
    draw.text((pos[0] - tw // 2, pos[1] - th // 2), text, font=font, fill=fill)

# ─── Hjørner ──────────────────────────────────────────────────────────────────
def draw_corners(img, draw, rank, sym, color):
    # Øverst til venstre
    draw.text((18, 18), rank, font=font_rank, fill=color)
    draw.text((20, 80), sym, font=font_suit_s, fill=color)

    # Nederst til høyre (rotert 180°)
    tmp = Image.new('RGBA', (80, 110), (0, 0, 0, 0))
    td = ImageDraw.Draw(tmp)
    td.text((8, 0), rank, font=font_rank, fill=color)
    td.text((10, 58), sym, font=font_suit_s, fill=color)
    tmp = tmp.rotate(180, expand=False)
    img.paste(tmp, (CARD_W - 90, CARD_H - 120), tmp)

# ─── Pip-layout for tallkort ──────────────────────────────────────────────────
PIP_LAYOUTS = {
    "A":  [(0.5, 0.5)],
    "2":  [(0.5, 0.18), (0.5, 0.82)],
    "3":  [(0.5, 0.18), (0.5, 0.5), (0.5, 0.82)],
    "4":  [(0.3, 0.18), (0.7, 0.18), (0.3, 0.82), (0.7, 0.82)],
    "5":  [(0.3, 0.18), (0.7, 0.18), (0.5, 0.5), (0.3, 0.82), (0.7, 0.82)],
    "6":  [(0.3, 0.18), (0.7, 0.18), (0.3, 0.5), (0.7, 0.5), (0.3, 0.82), (0.7, 0.82)],
    "7":  [(0.3, 0.18), (0.7, 0.18), (0.5, 0.34), (0.3, 0.5), (0.7, 0.5),
           (0.3, 0.82), (0.7, 0.82)],
    "8":  [(0.3, 0.18), (0.7, 0.18), (0.5, 0.34), (0.3, 0.5), (0.7, 0.5),
           (0.5, 0.66), (0.3, 0.82), (0.7, 0.82)],
    "9":  [(0.3, 0.16), (0.7, 0.16), (0.3, 0.38), (0.7, 0.38), (0.5, 0.5),
           (0.3, 0.62), (0.7, 0.62), (0.3, 0.84), (0.7, 0.84)],
    "10": [(0.3, 0.16), (0.7, 0.16), (0.5, 0.27), (0.3, 0.38), (0.7, 0.38),
           (0.3, 0.62), (0.7, 0.62), (0.5, 0.73), (0.3, 0.84), (0.7, 0.84)],
}

# ─── Lag ett kort ─────────────────────────────────────────────────────────────
def create_card(rank, suit_key):
    suit = SUITS[suit_key]
    color = suit["color"]
    sym = suit["sym"]

    img = Image.new('RGB', (CARD_W, CARD_H), WHITE)
    draw = ImageDraw.Draw(img)
    draw.rounded_rectangle((0, 0, CARD_W-1, CARD_H-1), radius=BORDER_RADIUS,
                           fill=WHITE, outline=BORDER_COL, width=3)

    draw_corners(img, draw, rank, sym, color)

    # Pip-område
    pip_l, pip_r = 90, CARD_W - 90
    pip_t, pip_b = 110, CARD_H - 110
    pw, ph = pip_r - pip_l, pip_b - pip_t

    if rank in PIP_LAYOUTS:
        use_font = font_big_ace if rank == "A" else font_pip
        for (cx_f, cy_f) in PIP_LAYOUTS[rank]:
            px = pip_l + int(cx_f * pw)
            py = pip_t + int(cy_f * ph)
            text_center(draw, (px, py), sym, use_font, color)
    else:
        # Bildekort (J, Q, K)
        fm = 70
        fx0, fy0, fx1, fy1 = fm, 120, CARD_W - fm, CARD_H - 120

        frame_col = (30, 60, 160) if color == BLACK else (160, 30, 30)
        bg_col    = (220, 230, 250) if color == BLACK else (255, 230, 230)

        draw.rounded_rectangle((fx0, fy0, fx1, fy1), radius=12,
                               fill=None, outline=frame_col, width=4)
        draw.rounded_rectangle((fx0+8, fy0+8, fx1-8, fy1-8), radius=8, fill=bg_col)

        # Diagonalt stripemønster
        stripe_col = (80, 110, 200) if color == BLACK else (220, 80, 80)
        for i in range(-10, 20):
            lx = fx0 + 8 + i * 28
            draw.line([(lx, fy0+8), (lx + (fy1-fy0), fy1-8)], fill=stripe_col, width=1)

        # Overmaling utenfor rammen for å klippe stripene
        # Venstre
        draw.rectangle((0, 0, fx0+8, CARD_H), fill=WHITE)
        # Høyre
        draw.rectangle((fx1-8, 0, CARD_W, CARD_H), fill=WHITE)
        # Topp
        draw.rectangle((0, 0, CARD_W, fy0+8), fill=WHITE)
        # Bunn
        draw.rectangle((0, fy1-8, CARD_W, CARD_H), fill=WHITE)

        # Tegn ramme og hjørner på nytt etter overmalingen
        draw.rounded_rectangle((0, 0, CARD_W-1, CARD_H-1), radius=BORDER_RADIUS,
                               fill=None, outline=BORDER_COL, width=3)
        draw_corners(img, draw, rank, sym, color)

        draw.rounded_rectangle((fx0, fy0, fx1, fy1), radius=12,
                               fill=None, outline=frame_col, width=4)
        draw.rounded_rectangle((fx0+8, fy0+8, fx1-8, fy1-8), radius=8, fill=None, outline=frame_col, width=2)

        # Stor bokstav + suit i midten
        text_center(draw, (CARD_W//2, CARD_H//2 - 30), rank, font_face_l, color)
        text_center(draw, (CARD_W//2, CARD_H//2 + 80), sym, font_face_sym, color)

    return img


def create_card_back():
    img = Image.new('RGB', (CARD_W, CARD_H), WHITE)
    draw = ImageDraw.Draw(img)

    draw.rounded_rectangle((0, 0, CARD_W-1, CARD_H-1), radius=BORDER_RADIUS,
                           fill=WHITE, outline=BORDER_COL, width=3)

    # Blå bakgrunn
    m = 12
    draw.rounded_rectangle((m, m, CARD_W-m-1, CARD_H-m-1),
                           radius=BORDER_RADIUS-4, fill=(20, 60, 140))

    # Indre ramme med gullkant
    m2 = 24
    draw.rounded_rectangle((m2, m2, CARD_W-m2-1, CARD_H-m2-1),
                           radius=12, fill=(15, 45, 110), outline=(200, 170, 50), width=3)

    # Diamantmønster
    ds = 18
    for row in range(m2+15, CARD_H-m2-10, ds*2):
        for col in range(m2+15, CARD_W-m2-10, ds*2):
            cx, cy = col + ds//2, row + ds//2
            pts = [(cx, cy-ds//2), (cx+ds//2, cy), (cx, cy+ds//2), (cx-ds//2, cy)]
            draw.polygon(pts, fill=(25, 55, 130), outline=(50, 100, 200))

    # Sentral dekorasjon
    cx, cy = CARD_W//2, CARD_H//2
    r = 55
    draw.ellipse((cx-r, cy-r, cx+r, cy+r), fill=(200, 170, 50), outline=(255, 220, 80), width=3)
    draw.ellipse((cx-r+10, cy-r+10, cx+r-10, cy+r-10), fill=(20, 60, 140), outline=(200, 170, 50), width=2)
    d = 20
    draw.polygon([(cx, cy-d), (cx+d, cy), (cx, cy+d), (cx-d, cy)], fill=(200, 170, 50))

    return img


# ─── Generer alle kort ────────────────────────────────────────────────────────
def main():
    count = 0
    for suit_key in ["S", "H", "D", "C"]:
        for rank in RANKS:
            img = create_card(rank, suit_key)
            fn = f"{suit_key}{rank}.jpg"
            img.save(os.path.join(OUT_DIR, fn), "JPEG", quality=95)
            count += 1
            print(f"  [{count:2d}/53] {rank} {SUITS[suit_key]['sym']} -> {fn}")

    img = create_card_back()
    img.save(os.path.join(OUT_DIR, "back.jpg"), "JPEG", quality=95)
    count += 1
    print(f"  [{count:2d}/53] Bakside -> back.jpg")
    print(f"\nFerdig! {count} kort lagret i {OUT_DIR}")

if __name__ == "__main__":
    main()

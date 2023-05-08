import secrets
import string
import os
import sys

# Imports for creating the JPEG images
import numpy
from PIL import Image

# Imports for generating the random PDFs
from reportlab.pdfgen import canvas
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.pdfbase import pdfmetrics
from reportlab.lib import colors


ascii_list = string.ascii_lowercase + string.ascii_uppercase + string.digits


def create_images():
    for i in range(100):
        array = numpy.random.rand(50, 50, 3) * 255
        image = Image.fromarray(array.astype('uint8')).convert('RGB')
        image.save('./images/out%000d.jpg' % i)


def create_pdfs():
    path = os.path.abspath(sys.argv[0])
    directory = os.path.dirname(path)
    os.chdir(directory + '/pdfs')
    for i in range(100):
        name = ''
        while name == '':
            name = ''.join(secrets.choice(ascii_list) for _ in range(secrets.randbits(k=4)))
        pdf = canvas.Canvas(name)
        pdf.setTitle(secrets.token_bytes(secrets.randbits(k=4)))

        font_list = pdf.getAvailableFonts()
        pdf.setFont(secrets.choice(font_list), secrets.randbits(k=4))
        
        count = secrets.randbits(k=4)
        for j in range(count):
            x1 = secrets.randbits(k=4)
            y1 = secrets.randbits(k=4)
            x2 = secrets.randbits(k=4)
            y2 = secrets.randbits(k=4)
            pdf.line(x1, y1, x2, y2)

        data = ''.join(secrets.choice(ascii_list) for _ in range(secrets.randbits(k=8)))
        color_dict = colors.getAllNamedColors()
        x, y = secrets.randbits(k=4), secrets.randbits(k=4)
        text = pdf.beginText(x, y)
        text.setFont(secrets.choice(font_list), secrets.randbits(k=4))
        text.setFillColor(color_dict[secrets.choice(list(color_dict.keys()))])
        text.textLine(data)
        pdf.drawText(text)

        pdf.save()

    os.chdir(directory)


def create_null_buf():
    for i in range(100):
        name = ''
        while name == '':
            name = ''.join(secrets.choice(ascii_list) for _ in range(secrets.randbits(k=4))) 
        with open(os.path.join('./null3d0ut', name), 'wb') as f:
            data = b'\x00' * secrets.randbits(k=8)
            f.write(data)


def main():
    create_images()
    create_pdfs()
    create_null_buf()


if __name__ == '__main__':
    main()

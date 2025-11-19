from PIL import Image, ImageFilter

before = Image.open("")
after = before.filter(ImageFilter.BoxBlur(1))
after.save("")
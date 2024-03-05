from PIL import Image
import matplotlib.pyplot as plt
import os

im = Image.open("kodim14.bmp")
ycbcr = im.convert('YCbCr')

print(ycbcr.getbands())

(y, cb, cr) = ycbcr.split()

plt.figure()
plt.title("Original")
plt.imshow(im)

plt.figure()
plt.subplot(131)
plt.title("Y")
plt.imshow(y, cmap="gray")
plt.subplot(132)
plt.title("Cb")
plt.imshow(cb, cmap="gray")
plt.subplot(133)
plt.title("Cr")
plt.imshow(cr, cmap="gray")

directory = os.path.dirname(os.path.abspath("kodim14.bmp"))

y.save(os.path.join(directory, "Y_channel.bmp"))
cb.save(os.path.join(directory, "Cb_channel.bmp"))
cr.save(os.path.join(directory, "Cr_channel.bmp"))

plt.show()
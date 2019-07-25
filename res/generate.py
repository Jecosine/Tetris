from PIL import Image
import numpy as np
imdata = np.array(Image.open("Untitled2.png").convert("L"))
content = ""
for i in range(40):
 for j in range(100):
   if imdata[i][j] == 0:
    content += "7" + " "
   else:
    content += str((255 - imdata[i][j]) % 7) + " "

 f = open("help_page.txt","wb")
 f.write(content)
 f.close()
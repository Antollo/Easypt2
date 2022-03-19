sudo apt install -y flex unzip

wget -O /tmp/llnextgen.deb "https://os.ghalkes.nl/LLnextgen/releases/llnextgen_0.5.5-1_amd64.deb"
sudo apt install -y /tmp/llnextgen.deb

wget -O /tmp/glfw.zip "https://github.com/glfw/glfw/releases/download/3.3.6/glfw-3.3.6.zip"
rm -r /tmp/glfw
mkdir /tmp/glfw
unzip -q /tmp/glfw.zip -d /tmp/glfw
(cd /tmp/glfw/glfw-3.3.6 && exec cmake -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF .)
(cd /tmp/glfw/glfw-3.3.6 && exec cmake --build . --config Release)

wget -O /tmp/glew.zip "https://netix.dl.sourceforge.net/project/glew/glew/2.2.0/glew-2.2.0.zip"
rm -r /tmp/glew
mkdir /tmp/glew
unzip -q /tmp/glew.zip -d /tmp/glew
(cd /tmp/glew/glew-2.2.0 && exec make glew.lib)

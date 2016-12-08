Author: Shelby Shuff (sshuff@mix.wvu.edu)
Author: Philip Fanelli (phfanelli@mix.wvu.edu)

To compile and run the project, navigate to the project folder and use the following terminal commands
mkdir build
cd build
cmake ../
make
./bin/draw_scene -fragment_shader_filepath=../fragment_shader.glsl -vertex_shader_filepath=../vertex_shader.glsl

Once running you should see two textured presents orbiting around a Christmas tree.

#/bin/sh

rsync -azP --exclude='*/node_modules/' ./webserver gg@ggshm.local:GravityGardens-SHM/smh_pi

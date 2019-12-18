
INTERNAL DOCUMENT (DO NOT RELEASE!)

Flag: wgmy{2fca11a75e1d472145c7dbf54dfc8102}

hints:
1) no, user agent technique is not going to help you today. you need to be a real robot!
2) dataset link: https://www.kaggle.com/c/dogs-vs-cats/data

HOW TO SOLVE
-------------

1) $ cd POC
2) Download train.zip 'Dogs vs Cats' dataset here: https://www.kaggle.com/c/dogs-vs-cats/data
3) $ unzip train.zip
4) Rearrange the images:

    $ cd train/
    $ mkdir cat dog
    $ mv cat*.jpg cat/
    $ mv dog*.jpg dog/
    $ cd ..

5) Install Tensorflow 2, preferably with GPU support
6) Run `tf-train.py` to train the model against the dataset
    - it should output `mobilenet-trained.h5`, our trained model
7) Edit `API_URL` in `tf-solve-ctf.py`
8) Run tf-solve-ctf.py and wait for the flag
9) ???
10) profit

HOW TO SETUP WEB SERVER
------------------------

1) Copy contents inside WEB-SERVER-PHP to your server (eq; /var/www/html/)
2) Download train.zip 'Dogs vs Cats' dataset here: https://www.kaggle.com/c/dogs-vs-cats/data
3) $ unzip train.zip
4) done
5) check log for participants suffering

Ah, and don't forget to disable directory listing:
ref: https://stackoverflow.com/a/31445273

note: no need to release anything to participants, just give them server URL


import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3' 

from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing import image
from tensorflow.keras.applications.mobilenet_v2 import preprocess_input

import requests
import numpy as np

import json
import tempfile
import base64
import glob


API_URL = "https://robot-captcha.wargames.my/api.php"


model = load_model("mobilenet-trained.h5")
print(model.summary())

def do_prediction(img_path):
    img = image.load_img(img_path, target_size=(224, 224))
    x = image.img_to_array(img)
    x = np.expand_dims(x, axis=0)
    x = preprocess_input(x)
    preds = model.predict(x)
    return 'dog' if preds > 0.80 else 'cat'


def main():

    s = requests.Session()

    # reset server state
    s.get("{}?reset_all".format(API_URL))

    i = 1
    while True:

        r = s.get("{}?req".format(API_URL))

        if "ALERT!" in r.text:
            print(r.text)
            break

        temp = tempfile.NamedTemporaryFile()
        temp.write(base64.b64decode(r.text))
        result = do_prediction(temp)

        r = s.get("{}?submitanswer={}".format(API_URL, result))

        if "ALERT!" in r.text:
            print(r.text)
            break

        r_json = json.loads(r.text)

        if r_json['status'] == 'Correct':
            r = s.get("{}?get_flag".format(API_URL))
            if 'ALERT!' not in r.text:
                print("Got true! Text = {}".format(r.text))
                break

        print("Trying {} -- Prediction Status: {}".format(i, r_json['status']))

        i += 1

if __name__ == '__main__':
    main()

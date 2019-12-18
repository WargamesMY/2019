<?php

session_start();

# download the dataset here: https://www.kaggle.com/c/dogs-vs-cats/data

# options
define("IMG_FOLDER", "train/");  # dont forget to disable dir listing
define("NUM_OF_QUESTIONS", 500);
define("CORRECT_PERCENTAGE_NEEDED", 0.90);
define("TIME_LIMIT", 100); # in seconds
define("FLAG", "wgmy{2fca11a75e1d472145c7dbf54dfc8102}");

function get_new_image()
{
    $scanned_directory = array_diff(scandir(IMG_FOLDER), array('..', '.'));

    $rand_element = array_rand($scanned_directory, 1);
    $rand_element = $scanned_directory[$rand_element];

    $thumb = new Imagick();
    $thumb->readImage(IMG_FOLDER. $rand_element);

    /*
    
    // disabled, takes too much server resources
    
    $rand_transform = rand(1, 3);

    switch($rand_transform)
    {
        case 1: {
            $degree = rand(15, 25) * [-1, 1][rand(0,1)];
            $thumb->rotateimage(new ImagickPixel('#00000000'), $degree);
            break;
        }
        
        case 2:
            $thumb->addNoiseImage(imagick::NOISE_GAUSSIAN);
            break;
        default:
            break;
    }

    # to prevent hash search, we do some transformation
    $thumb->resizeImage(400, 245, Imagick::FILTER_CATROM,1);*/

    $thumb->setImageCompression(Imagick::COMPRESSION_JPEG);
    $thumb->setImageCompressionQuality(20);

    return [
        substr($rand_element, 0, 3),
        base64_encode($thumb->getImageBlob()),
    ];
}

function get_correct_required()
{
    return round((float)NUM_OF_QUESTIONS * CORRECT_PERCENTAGE_NEEDED);
}

function reset_session_var()
{
    $_SESSION['start_time'] = time();
    $_SESSION['attempt_cnt'] = 0;
    $_SESSION['correct_cnt'] = 0;
    $_SESSION['answer'] = '';
}

#
# initialize var
#

if (!isset($_SESSION['start_time']) || !isset($_SESSION['attempt_cnt']) || !isset($_SESSION['correct_cnt']) || !isset($_SESSION['answer']))
{
    reset_session_var();
}

#
# API: req
#   server will send randomly selected cat or dog image in base64
#

if (isset($_GET['req']))
{
    if ($_SESSION['attempt_cnt'] > NUM_OF_QUESTIONS) {
        die("ALERT! You have reached maximum attempt! Refresh this page to restart.");
    }

    if (time() - $_SESSION['start_time'] > TIME_LIMIT) {
        die("ALERT! You have crossed the time limit " . TIME_LIMIT . " seconds. Refresh this page to restart.");
    }

    if ($_SESSION['correct_cnt'] >= get_correct_required()) {
        die("ALERT! You have answered >= " . get_correct_required() . " correctly! Request for ?get_flag to get the flag.");
    }

    if (!empty($_SESSION['answer'])) {
        die("ALERT! You must answer current question before requesting new one!");
    }

    $_SESSION['attempt_cnt']++;

    list($_SESSION['answer'], $img_encoded) = get_new_image();

    die($img_encoded);
}

#
# API: submitanswer
#  client will send correct answer here
#

else if (isset($_GET['submitanswer']) && !empty($_GET['submitanswer']))
{
    if ($_SESSION['attempt_cnt'] > NUM_OF_QUESTIONS) {
        die("ALERT! You have reached maximum attempt! Refresh this page to restart.");
    }

    if (time() - $_SESSION['start_time'] > TIME_LIMIT) {
        die("ALERT! You have crossed the time limit " . TIME_LIMIT . " seconds. Refresh this page to restart.");
    }

    if (empty($_SESSION['answer'])) {
        die("ALERT! Chill bro.");
    }

    if ($_SESSION['correct_cnt'] >= get_correct_required()) {
        die("ALERT! You have answered >= " . get_correct_required() . " correctly! Request for ?get_flag to get the flag.");
    }

    $is_correct = $_GET['submitanswer'] === $_SESSION['answer'];

    if ($is_correct) {
        $_SESSION['correct_cnt']++;
    }

    # reset prev answer
    $_SESSION['answer'] = '';

    $return_val = [
        "status" => $is_correct ? "Correct" : "False",
        "attempt_cnt" => $_SESSION['attempt_cnt'],
        "correct_cnt" => $_SESSION['correct_cnt']
    ];

    die(json_encode($return_val));
}

#
# API: get_flag
#  server will send flag if correct amount is reached
#

else if (isset($_GET['get_flag']))
{
    if (time() - $_SESSION['start_time'] > TIME_LIMIT) {
        die("ALERT! You have crossed the time limit " . TIME_LIMIT . " seconds. Refresh this page to restart.");
    }

    if ($_SESSION['correct_cnt'] < get_correct_required()) {
        die("ALERT! You have not reached >= " . get_correct_required() . " correct yet!");
    }
    
    die(FLAG);
}

#
# API: get_options
#  server will send all options value
#

else if (isset($_GET['get_options']))
{
    die(json_encode(
        [
            "num_question" => NUM_OF_QUESTIONS,
            "correct_required" => get_correct_required(),
            "time_limit" => TIME_LIMIT
        ]
    ));
}

#
# API: reset
#  server will reset all the variables
#

else if (isset($_GET['reset_all']))
{
    reset_session_var();
}

?>

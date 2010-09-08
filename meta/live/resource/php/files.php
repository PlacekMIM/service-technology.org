<?php

  require_once 'session.php';

/**
 * Gets an array of file names, copies the files to the working
 * directory and returns an structured array with information on the 
 * files:
 * 
 *  $a = prepareFiles( array ("foo/file1.bla", "bar/file2.blob") );
 * 
 *  $a => (
              [foo/file1] => Array
                  (
                      // original dirname
                      [dirname] => foo
                      // full filename
                      [basename] => file1.bla
                      // file extension
                      [extension] => bla
                      // filename without extension
                      [filename] => file1
                      // actual place in working directory (based on session id)
                      [residence] => /tmp/live/12ea9bc4df7609ee32f88a07a2714220/file1.bla
                      // link for downloading file
                      [link] => /live/getfile.php?file=file1.bla&amp;id=12ea9bc4df7609ee32f88a07a2714220
                  )

              [bar/file2.blob] => Array
                  (
                      [dirname] => bar
                      [basename] => file2.blob
                      [extension] => blob
                      [filename] => file2
                      [residence] => /tmp/live/12ea9bc4df7609ee32f88a07a2714220/file2.blob
                      [link] => /live/getfile.php?file=file2.blob&amp;id=12ea9bc4df7609ee32f88a07a2714220
                  )

          )
 */
 
function prepareFiles($fileArray)
{
  $result = array();
  
  foreach($fileArray as $file)
  {
    // returns "basename", "extension", "dirname", and "filename"
    $info = pathinfo($file);
    
    $result[$file] = $info;
    $result[$file]["residence"] = $_SESSION["dir"]."/".$info["basename"];
    $result[$file]["link"] = getLink($info["basename"]);
   
    copy($file, $result[$file]["residence"]);
  }

  return $result;
}

function prepareFile($file)
{  
  $result = array();
  // returns "basename", "extension", "dirname", and "filename"
  $info = pathinfo($file);
  
  $result[$file] = $info;
  $result[$file]["residence"] = $_SESSION["dir"]."/".$info["basename"];
  $result[$file]["link"] = getLink($info["basename"]);
 
  copy($file, $result[$file]["residence"]);

  return $result;
}

// create a string representing the URI for downloading a file
function getLink($file)
{
  return LIVEBASE."getfile.php?file=".urlencode(basename($file))."&amp;id=".urlencode($_SESSION["uid"]);
}

// function, which creates an image for a $file and actually writes some HTML code
// rest of the funtionality inside getimage.php
function drawImage($file, $thumbsize = 300)
{
  static $imagecounter = 0;
  $imagecounter++;

  $info = pathinfo($file);

  $callstring = LIVEBASE."getimage.php?file=".urlencode($file)."&thumbnail_size=".$thumbsize."&id=".urlencode($_SESSION["uid"]);
  $thumblink = LIVEBASE."getfile.php?file=".urlencode("thumb_".$file.".png")."&amp;id=".urlencode($_SESSION["uid"]);
  $label = $info["filename"];

  echo '<script type="text/javascript">'."\n";
  echo '<!--'."\n";
  echo '  $(document).ready(function(){'."\n";
  echo '    $(\'#thumb'.$imagecounter.'\').load(\''.$callstring.'\');'."\n";
  echo '    $(\'#thumb'.$imagecounter.'\').fadeIn("slow");'."\n";
  echo '  });'."\n";
  echo '// -->'."\n";
  echo '</script>'."\n";
    
  echo '<div class="dotimg" id="thumb'.$imagecounter.'">';
  echo '  <img src="resource/images/spinner.gif" alt="rendering image" /><br />rendering image...';
  echo '</div>';

}

/**
* Similar to prepareFiles, but instead of copying an array of files,
* one single file is created.
* Return value as above.
*/
function createFile($file, $content = "")
{
  $result = array();
  
  // returns "basename", "extension", "dirname", and "filename"
  $info = pathinfo($file);
    
  $result[$file] = $info;
  $result[$file]["residence"] = $_SESSION["dir"]."/".$info["basename"];
  $result[$file]["link"] = getLink($info["basename"]);
   
  $handle = fopen($result[$file]["residence"], "w+");
  fwrite($handle, stripslashes($content));
  fclose($handle);

  return $result;
}

// do not call the page without POST request, or only if session is 
// already set to Marlene (all information about services available)
if ( ! isset($_REQUEST) && ! isset($_SESSION[$tool]))
{
  // direct call of this page -> return to main page
  header("Location: index.html#$tool");
  exit;
}
else
{
  // new request?
  if ( isset($_REQUEST) && ! empty($_REQUEST))
  {
    $set = false;
    
    if ( ! strcmp($_REQUEST["input_type"], 'example') )
    {
      // remember name of example in session
      $_SESSION[$tool] = "$tool/".$_REQUEST["input_example"].'.bpel';
      $set = true;
    }
    if ( ! strcmp($_REQUEST["input_type"], 'uploaded') )
    {
      $_SESSION[$tool] = $_REQUEST["input_uploaded"];
      $set = true;
    }
    if ( ! strcmp($_REQUEST["input_type"], 'url') )
    {
      $_SESSION[$tool] = $_REQUEST["input_url"];
      $set = true;
    }
    if ( ! strcmp($_REQUEST["input_type"], 'given') )
    {
      $_SESSION[$tool] = $_REQUEST["input_given"];
      $set = true;
    }
    
    if ($set)
    {
      $_SESSION["input_type"] = $_REQUEST["input_type"];
      $_SESSION["patterns"] = $_REQUEST["patterns"];
      $_SESSION["format"] = $_REQUEST["format"];
      $_SESSION["reduce"] = $_REQUEST["reduce"];
      
      unset($_REQUEST);
      // redirect to self, make back/forward buttons work without 
      // resending the request
      header("Location: ".$_SERVER["PHP_SELF"]);
    }
  }
}

/*
 * Main part: This file MUST be included into any tool script. We process the given request variable
 * to set or create files respectively.
 */

//$fileinfo = array();
//
//// in case an example file was chosen from drop-down
//if ($_REQUEST["input_type"] == 'example')
//{
//  $f = $_REQUEST["input_example"];
//  $fileinfo = createFile($f);
//  
//  copy($f, $fileinfo[$f]["residence"]);
//}
//
//// in case an URL was given
//if ($_REQUEST["input_type"] == 'url')
//{
//  $f = $_REQUEST["input_url"];
//  $fileinfo = createFile($f);
//  
//  $fileinfo[$f]["link"] = $f;
//  
//  $download = $download = 'wget \''.$url.'\' -O '.$fileinfo[$f]["residence"];
//  system($download);
//}
//
//// in case a file was uploaded
//if ($_REQUEST["input_type"] == 'uploaded')
//{
//  $f = $_REQUEST["input_uploaded"];
//  $fileinfo = createFile($f);
//  
//  move_uploaded_file($_FILES['input_file']['tmp_name'], $fileinfo[$f]["residence"]);
//}
//
//// in case a file was given
//if ($_REQUEST["input_type"] == 'given')
//{
//  $f = $_REQUEST["input_given"];
//  $fileinfo = createFile('given_'.rand().rand(), $f);
//}

?>

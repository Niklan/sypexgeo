<?php

/**
 * @file
 * Drush integration.
 */

define('SYPEXGEO_PHP_LIBRARY_DOWNLOAD_URL', 'https://sypexgeo.net/files/SxGeo22_API.zip');

/**
 * Implementation of hook_drush_command().
 */
function sypexgeo_drush_command() {
  $items = array();

  $items['sypexgeo'] = array(
    'callback' => 'drush_sypexgeo_download',
    'description' => dt('Download and install the sypexgeo library.'),
    // We need it for saving files as managed.
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function sypexgeo_drush_help($section) {
  switch ($section) {
    case 'drush:sypexgeo':
      return dt('Download and install the sypexgeo library.');
  }
}

/**
 * Implements drush_MODULE_post_pm_enable().
 */
function drush_sypexgeo_post_pm_enable() {
  $modules = func_get_args();
  if (in_array('sypexgeo', $modules)) {
    drush_sypexgeo_download();
  }
}

/**
 * Main code for drush command.
 */
function drush_sypexgeo_download() {
  $variants = array(
    'library' => dt('Download and install SypexGeo PHP library'),
  );
  $type = drush_choice($variants, dt('What do you want to do?'));

  switch ($type) {
    case 'library':
      drush_sypexgeo_download_library();
      break;

    default:
      // Just do nothing.
      break;
  }
}

/**
 * Download SypexGeo PHP Library.
 */
function drush_sypexgeo_download_library() {
  $path_to_libraries = 'sites/all/libraries';

  // Creat libraries folder is missing.
  if (!is_dir($path_to_libraries)) {
    drush_op('mkdir', $path_to_libraries);
    drush_log(dt('Directory @path was created', array('@path' => $path_to_libraries)), 'notice');
  }

  $olddir = getcwd();
  chdir($path_to_libraries);
  // If already exist we reinstall it.
  if (is_dir('sypexgeo')) {
    drush_delete_dir('sypexgeo', TRUE);
  }
  drush_op('mkdir', 'sypexgeo');
  chdir('sypexgeo');

  if ($filepath = drush_download_file(SYPEXGEO_PHP_LIBRARY_DOWNLOAD_URL)) {
    // SxGeo22_API.zip
    $filename = basename($filepath);
    drush_tarball_extract($filename);
  }

  if (is_file('SxGeo.php')) {
    drush_log(dt('SypexGeo PHP library has been installed in @path', array('@path' => $path_to_libraries)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the SypexGeo PHP library to @path', array('@path' => $path_to_libraries)), 'error');
  }

  chdir($olddir);
}
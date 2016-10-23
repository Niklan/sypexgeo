<?php

/**
 * @file
 * Drush integration.
 */

define('SYPEXGEO_PHP_LIBRARY_DOWNLOAD_URL', 'https://sypexgeo.net/files/SxGeo22_API.zip');
define('SYPEXGEO_CITY_DB_DOWNLOAD_URL', 'https://sypexgeo.net/files/SxGeoCity_utf8.zip');

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
    'db_city' => dt('Download and install SypexGeo City database'),
    'lib_city' => dt('Download and install SypexGeo City database and SypexGeo PHP library'),
  );
  $type = drush_choice($variants, dt('What do you want to do?'));

  switch ($type) {
    case 'library':
      drush_sypexgeo_download_library();
      break;

    case 'db_city':
      drush_sypexgeo_download_db_city();
      break;

    case 'lib_city':
      drush_sypexgeo_download_library();
      drush_sypexgeo_download_db_city();
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

  // Create libraries folder is missing.
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

/**
 * Download SypexGeo PHP Library.
 */
function drush_sypexgeo_download_db_city() {
  $path_to_database_folder = 'public://sypexgeo';
  $sypexgeo_fid = variable_get('sypexgeo_db_city_fid', '');

  // Create libraries folder is missing.
  if (!is_dir($path_to_database_folder)) {
    drush_op('mkdir', $path_to_database_folder);
    drush_log(dt('Directory @path was created', array('@path' => $path_to_database_folder)), 'notice');
  }

  $olddir = getcwd();

  // Create temporary folder for file manipulations.
  $download_path = drush_tempdir();
  chdir($download_path);
  if ($filepath = drush_download_file(SYPEXGEO_CITY_DB_DOWNLOAD_URL, $download_path . '/SxGeo.zip')) {
    $filename = basename($filepath);
    drush_tarball_extract($filename);

    $database = fopen($download_path . '/SxGeoCity.dat', 'r');
    chdir($olddir);
    $database_copied = file_save_data($database, $path_to_database_folder .'/SxGeoCity.dat');
    fclose($database);

    if ($database_copied) {
      // Rename file because somehow name and extension are broken from temp.
      //file_move($database_copied, $path_to_database_folder . 'SxGeoCity.Dat');

      if ($sypexgeo_fid > 0) {
        $old_file = file_load($sypexgeo_fid);
        file_usage_delete($old_file, 'sypexgeo', 'database_city', $old_file->fid);
      }

      file_usage_add($database_copied, 'sypexgeo', 'database_city', $database_copied->fid);
      variable_set('sypexgeo_db_city_fid', $database_copied->fid);
      variable_set('sypexgeo_db_city_last_update', time());
      drush_log(dt('SypexGeo City database has been installed in @path', array('@path' => $path_to_database_folder)), 'success');
    }
    else {
      drush_log(dt('Drush was unable to install the SypexGeo City database to @path', array('@path' => $path_to_database_folder)), 'error');
    }
  }

  chdir($olddir);
}

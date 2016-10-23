<?php

/**
 * @file
 * Administration interface for module settings and configurations.
 */

/**
 * SypexGeo db settings.
 */
function sypexgeo_admin_database_settings($form, &$form_state) {
  $form = array();

  if (($library = libraries_load('sypexgeo')) && empty($library['loaded'])) {
    drupal_set_message(t('SypexGeo PHP library is not installed. Please install it before using module.'), 'warning');
    $form['how_to_install_php_library'] = array(
      '#type' => 'markup',
      '#markup' => t('Go to <a href="@href" target="_blank">download page</a> and download SyepxGeo for PHP. Then extract and place SxGeo.php file to /sites/all/libraries/sypexgeo. Or you can use drush command <code>drush sypexgeo</code>.',
        array(
          '@href' => 'https://sypexgeo.net/ru/download/',
        )),
    );
  }

  $db_description_download = '<p>' . t('You can <a href="@href" target="_blank">download</a> latest version of database on official site. Or you can use drush command <code>drush sypexgeo</code> to update or download fresh database.', array('@href' => 'https://sypexgeo.net/ru/download/')) . '</p>';
  $last_update_time = variable_get('sypexgeo_db_city_last_update', 0);
  if ($last_update_time) {
    $db_description_last_update = '<p>' . t('Current database uploaded on @date', array('@date' => format_date($last_update_time))) . '</p>';
  }
  else {
    $db_description_last_update = '<p><strong>' . t('The database is not uploaded before.') . '</strong></p>';
  }

  $form['sypexgeo_city_db'] = array(
    '#title' => 'SypexGeo City',
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => $db_description_last_update . $db_description_download,
    '#default_value' => variable_get('sypexgeo_db_city_fid', ''),
    '#upload_location' => 'public://sypexgeo/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('dat'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submission callback for database settings form.
 */
function sypexgeo_admin_database_settings_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $db_city_old_fid = variable_get('sypexgeo_db_city_fid', '');

  // Save and update information about file.
  if ($values['sypexgeo_city_db'] > 0 && $values['sypexgeo_city_db'] != $db_city_old_fid) {
    // At first we must remove old one.
    if ($db_city_old_fid > 0) {
      $old_file = file_load($db_city_old_fid);
      file_usage_delete($old_file, 'sypexgeo', 'database_city', $old_file->fid);
    }

    $import_file = file_load($values['sypexgeo_city_db']);
    file_save($import_file);
    file_usage_add($import_file, 'sypexgeo', 'database_city', $import_file->fid);
    variable_set('sypexgeo_db_city_fid', $import_file->fid);
    variable_set('sypexgeo_db_city_last_update', time());
  }
}

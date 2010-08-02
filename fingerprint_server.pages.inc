<?php

/**
 * Callback for user/%user/site_fingerprint
 */
function site_fingerprint_credentials($user) {
  $query = db_query('SELECT * FROM {fingerprint_keys} WHERE uid=%d', $user->uid);
  $key_table = array();
  while($row = db_fetch_object($query)) {
     $key_table[] = array($row->project_name, $row->kid);
  }

  if (!empty($key_table)) {
    $header = array(t('Project name'), t('Key'));
    return theme('table', $header, $key_table);
  }

  return t('No key associated with your account');
}


/**
 * Callback for user/%user/fingerprint/add
 *
 * @defgroup fingerprint_server_key_add Key association forms
 * @{
 * Associates an existing key to the current user
 */
function fingerprint_server_key_add() {
  $form = array();

  $form['project_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Project name'),
    '#size' => 64,
    '#maxlength' => 64,
    '#required' => TRUE,
  );

  $form['kid'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#description' => t('The fingerprint key for your site. You can look it up at admin/settings/fingerprint .'),
    '#size' => 42,
    '#maxlength' => 42,
    '#required' => TRUE,
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Add'));

  return $form;
}




/**
 * Saves the fignerprint key association
 * @global $user
 */
function fingerprint_server_key_add_submit($form, &$form_state) {
  global $user;

  $fingerprint_key = new stdClass();
  $fingerprint_key->kid = $form_state['values']['kid'];
  $fingerprint_key->project_name = $form_state['values']['project_name'];
  $fingerprint_key->uid = $user->uid;
  drupal_write_record('fingerprint_keys', $fingerprint_key);
}
/**
 * @} fingerprint_server_key_add
 */


/**
 * Callback for admin/settings/fignerprint_server
 */
function fingerprint_server_settings() {
  $form = array();

  $form['fingerprint_server_default_keys'] = array(
    '#type' => 'textarea',
    '#title' => t('Default keys'),
    '#default_value' => variable_get('fingerprint_server_default_keys', ''),
  );

  return system_settings_form($form);
}


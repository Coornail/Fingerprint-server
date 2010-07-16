<?php

/**
 * Callback for user/%user/site_fingerprint
 */
function site_fingerprint_credentials($user) {
  $api_key = db_result(db_query('SELECT kid FROM {services_keys} WHERE domain="%s"', $user->name));
  if (!$api_key) {
    $api_key = 'You don\'t have an api key. If you think it\'s our fault, please contact us: '. variable_get('site_mail', '');
  }
  $list[] = '<strong>Server: </strong>'. url('<front>', array('absolute' => TRUE));
  $list[] = '<strong>xmlrpc path: </strong>' . '?q=services/xmlrpc';
  $list[] = '<strong>Username: </strong>'. $user->name;
  $list[] = '<strong>Password: </strong> Drupal site password.';
  $list[] = '<strong>Api key: </strong>'. $api_key;
  return theme('item_list', $list);
}


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


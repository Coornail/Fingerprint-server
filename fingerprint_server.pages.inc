<?php

/**
 * Callback for user/%user/site_fingerprint
 */
function site_fingerprint_credentials($user) {
  $query = db_query('SELECT * FROM {fingerprint_keys} WHERE uid=%d', $user->uid);
  $key_table = array();
  while($row = db_fetch_object($query)) {
     $key_table[] = array($query->project_name, $query->kid);
  }

  if (!empty($key_table)) {
    $header = array(t('Project name'), t('Key'));
    return theme('table', $header, $key_table);
  }

  return t('No key associated with your account');
}

function fingerprint_server_key_add() {
  return array();
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


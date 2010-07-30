<?php

/** Service fingerprint.getDefaultKeys **/
function fingerprint_services_get_default_keys_access() {
  return TRUE;
}

function fingerprint_services_get_default_keys() {
  return explode("\n", variable_get('fingerprint_server_default_keys', ''));
}


/** Service fingerprint.supportedVersion **/
function fingerprint_services_supported_version_access() {
  return TRUE;
}

/**
 * Returns the supported client version numbers
 */
function fingerprint_services_supported_version() {
  return array('0.3');
}


/** Service fingerprint.isAnonymousKey **/
function fingerprint_services_is_anonymouskey_access() {
  return TRUE;
}

function fingerprint_services_is_anonymouskey($key) {
  $keys = fingerprint_services_get_default_keys();
  return (bool)in_array($key, $keys);
}


/**
 * Service fingerprint.registerKey
 *
 * Registers a key and sends back the key and the domain
 **/
function fingerprint_services_register_key_access() {
  return TRUE;
}

function fingerprint_services_register_key() {
  module_load_include('inc', 'services_keyauth', 'services_keyauth.admin');
  $key = array();
  $key['kid'] = md5(uniqid(mt_rand(), TRUE));
  $key['domain'] = md5(uniqid(mt_rand(), TRUE));
  $key['title'] = $key['domain'];
  $key['method_access'] = array('system.connect', 'fingerprint.save');

  $ret = services_keyauth_admin_keys_save(&$key);
  if ($ret == SAVED_NEW) {
    return $key;
  }

  return FALSE;
}


 /**
  * Service fingerprint.save
  *
  * This is required, so we won't allow people to save whatever in the node they want
  * It's also important because of the user matching.
  **/
function fingerprint_services_save_access() {
  return TRUE;
}


function fingerprint_services_save($fingerprint) {
  $node = new stdClass();
  if (isset($fingerprint['nid'])) {
    $node->nid = $fingerprint['nid'];
  }
  $node->title = $fingerprint['title'];
  $node->body = $fingerprint['body'];
  $node->type = 'fingerprint';

  node_save($node);
  return $node;
}

<?php
// $Id$

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
  return array('0.2');
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

/**
 * Registers a random key for a random domain
 *
 * @return
 *   An associative array with the services settings
 *   - kid           The generated key
 *   - domain        The generated domain
 *   - title         The generated domain
 *   - method_access Array of accessible methods trough xmlrpc
 */
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


/**
 * Saves a fingerprint node
 * It will only save the necessary information from the node.
 * If the key is iset in fingerprint_keys, it will save the user
 *
 * @param $fingerprint
 *   Fingerprint node object
 */
function fingerprint_services_save($fingerprint) {
  // If the key auth module is not configured we might get all the authentication data
  if (count(func_get_args()) > 1) {
    return services_error(t('Server is not configured yet'), 503);
  }

  $node = new stdClass();

  // Check if the nid is sent by the same user as last time
  if (isset($fingerprint['nid'])) {
    $kid = db_result(db_query('SELECT kid FROM {fingerprint_node} WHERE nid="%d"', $fingerprint['nid']));
    if ($kid == $fingerprint['fingerprint_key']) {
      $node->nid = $fingerprint['nid'];
    }
  }

  $node->title = check_plain($fingerprint['title']);
  $node->body = $fingerprint['body'];
  $node->type = 'fingerprint';
  $node->status = TRUE; // by default it's public

  $uid = db_result(db_query('SELECT uid FROM {fingerprint_keys} WHERE kid="%s"', $fingerprint['fingerprint_key']));
  if ($uid) {
    $node->uid = $uid;
  }

  node_save($node);
  if ($node !== FALSE) {
    // Save the key to the node id
    $fingerprint_node = new stdClass();
    $fingerprint_node->nid = $node->nid;
    $fingerprint_node->kid = $fingerprint['fingerprint_key'];
    drupal_write_record('fingerprint_node', $fingerprint_node);

    return $node;
  }

  return services_error(t('Failed to save node'), 503);
}

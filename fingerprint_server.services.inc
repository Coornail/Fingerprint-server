<?php

/** Service fingerprint.getDefaultKeys **/
function fingerprint_services_get_default_keys_access() {
  return TRUE;
}

function fingerprint_services_get_default_keys() {
  return explode("\n", variable_get('fingerprint_server_default_keys'));
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


/** Services fingerprint.isAnonymousKey **/
function fingerprint_services_is_anonymouskey_access() {
  return TRUE;
}

function fingerprint_services_is_anonymouskey($key) {
  $keys = fingerprint_services_get_default_keys();
  return (bool)in_array($key, $keys);
}

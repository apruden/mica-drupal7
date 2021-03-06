<?php
/**
 * Copyright (c) 2016 OBiBa. All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

$path_module_commons = drupal_get_path('module', 'obiba_mica_commons');
include_once($path_module_commons . '/includes/obiba_mica_commons_temp_file_resource.inc');

/**
 * Get the data access request form config.
 */
function obiba_mica_data_access_request_form_get_config() {
  $mica_client = new MicaDataAccessRequest();
  $config = $mica_client->getFormDefinition();
  drupal_json_output($config);
}

/**
 * Actions on data access requests.
 */
function obiba_mica_data_access_requests_action() {

  switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':
      $mica_client = new MicaDataAccessRequests(NULL, 'METHOD_GET');
      $data_access_request = $mica_client->getRequests();
      drupal_json_output($data_access_request);
      break;
    case 'POST':
      $mica_client = new MicaDataAccessRequests(NULL, 'METHOD_POST');
      $data_access_request = $mica_client->dataAccessSave(file_get_contents('php://input'), '/data-access-requests');
      drupal_json_output($data_access_request->body);
      break;
  }

}

/**
 * Actions on comments of data access request.
 */
function obiba_mica_data_access_request_comments_action($id_data_request) {
  switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':
      $mica_client = new MicaDataAccessRequestComments(NULL, 'METHOD_GET');
      $data_access_request = $mica_client->getComments($id_data_request, '/data-access-request');
      drupal_json_output($data_access_request);
      break;

    case 'POST':
      $mica_client = new MicaDataAccessRequestComments(NULL, 'METHOD_POST');
      $data_access_request = $mica_client->createComment(file_get_contents('php://input'), $id_data_request, '/data-access-request');
      drupal_json_output($data_access_request);
      break;
  }

}

/**
 * Actions on comment of data access request.
 */
function obiba_mica_data_access_request_comment_action($id_data_request, $comment_id) {

  switch ($_SERVER['REQUEST_METHOD']) {
    case 'PUT':
      $mica_client = new MicaDataAccessRequestComment(NULL, 'METHOD_PUT');
      $data_access_request = $mica_client->saveComment($id_data_request, $comment_id, file_get_contents('php://input'), '/data-access-request');
      drupal_json_output($data_access_request);
      break;

    case 'DELETE':
      $mica_client = new MicaDataAccessRequestComment(NULL, 'METHOD_DELETE');
      $data_access_request = $mica_client->deleteComment($id_data_request, $comment_id, '/data-access-request');
      drupal_json_output($data_access_request);
      break;
  }

}

/**
 * Get data access request attachments.
 */
function obiba_mica_data_access_request_attachment($id_data_request, $id_file) {
  $dataset_resource = new MicaDataAccessRequest(NULL, 'METHOD_GET');
  $responses = $dataset_resource->downloadAttachment($id_data_request, $id_file);
  if (!empty($responses)) {
    drupal_add_http_header('Content-Disposition', $responses['headers']['Content-Disposition'][0]);
    echo $responses['response']->body;
    flush();
    // Stop drupal execution error : 'Headers already sent'.
    die();
  }
}

/**
 * Get data access request attachments.
 */
function obiba_mica_data_access_request_form_attachment($file_name, $file_id) {
  $dataset_resource = new MicaDataAccessRequest(NULL, 'METHOD_GET');
  $responses = $dataset_resource->downloadFormAttachment(drupal_get_query_parameters()['path'], $file_name, $file_id);
  if (!empty($responses)) {
    drupal_add_http_header('Content-Disposition', $responses['headers']['Content-Disposition'][0]);
    echo $responses['response']->body;
    flush();
    // Stop drupal execution error : 'Headers already sent'.
    die();
  }
}

/**
 * Download a pdf of the data access request.
 */
function obiba_mica_data_access_request_download_pdf($id_data_request) {
  if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    $dataset_resource = new MicaDataAccessRequest(NULL, 'METHOD_GET');
    $responses = $dataset_resource->downloadPdf($id_data_request, $_GET['lang']);
    if (!empty($responses)) {
      drupal_add_http_header('Content-Disposition', $responses['headers']['Content-Disposition'][0]);
      echo $responses['response']->body;
      flush();
      // Stop drupal execution error : 'Headers already sent'.
      die();
    }
  }
}

/**
 * Actions on data access request.
 */
function obiba_mica_data_access_request_action($id_data_request) {
  switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':
      $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_GET');
      $data_access_request = $mica_client->getRequest($id_data_request);
      if (empty($data_access_request)) {
        return FALSE;
      }
      drupal_json_output($data_access_request);
      break;

    case 'PUT':
      $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_PUT');
      $response = $mica_client->dataAccessSave(file_get_contents('php://input'), '/data-access-request', $id_data_request);

      if (isset($response) && $response->responseCode != 204) drupal_json_output($response); #MK-966: apache proxy ignores body on 204
      break;

    case 'DELETE':
      obiba_mica_data_access_request_delete_request_access($id_data_request);
      break;
  }
}

/**
 * Update attachments of the data access request.
 *
 * @param $id_data_request
 */
function obiba_mica_data_access_request_attachments_update($id_data_request) {
  if ($_SERVER['REQUEST_METHOD'] === 'PUT') {
      $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_PUT');
      $response = $mica_client->saveAttachments(file_get_contents('php://input'), $id_data_request);
      if (isset($response) && $response->responseCode != 204) drupal_json_output($response); #MK-966: apache proxy ignores body on 204
  }
}

/**
 * Delete the data access request.
 */
function obiba_mica_data_access_request_delete_request_access($id_request) {
  $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_DELETE');
  $data_access_request = $mica_client->deleteRequest($id_request);
  drupal_json_output($data_access_request);
}

/**
 * Redirection callaBack on not permitted actions, detected in AngularJs app.
 */
function obiba_mica_data_access_request_redirect($operation, $id_request) {
  global $base_url;
  $url = $base_url . '/' . 'mica/data_access/request#/' . $operation . '/' . $id_request;
  header('Location: ' . $url, TRUE);
  drupal_exit($url);
}

/**
 * Update the data access request.
 */
function obiba_mica_data_access_request_update_request($id_request, $status) {
  $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_PUT');
  $data_access_request = $mica_client->statusUpdateRequest($id_request, $status);
  drupal_json_output($data_access_request);
}

/**
 * Update the data access request attachments.
 */
function obiba_mica_data_access_request_upload_attachment() {
  $mica_client = new MicaTempFileResource(NULL, 'METHOD_POST');
  $response = $mica_client->uploadFile($_FILES);
  drupal_json_output($response);
}

/**
 * GEt the temp data access request attachments.
 */
function obiba_mica_data_access_request_get_attachment($id_file) {
  switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':
      $mica_client = new MicaTempFileResource(NULL, 'METHOD_GET');
      $response = $mica_client->getFile($id_file);
      drupal_json_output($response);
      break;
    case 'DELETE':
      $mica_client = new MicaTempFileResource(NULL, 'METHOD_DELETE');
      $response = $mica_client->deleteFile($id_file);
      drupal_json_output($response);
      break;
  }
}

function obiba_mica_data_access_request_user_profile($user_id) {
  $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_GET');
  $response = $mica_client->getUserProfile($user_id);
  $user_profile =
    obiba_mica_data_access_request_user_profile_attribute($response->attributes);
  drupal_json_output(array('profile_html' => $user_profile));
}

function obiba_mica_data_access_request_user_profile_attribute($user_attributes) {
  $html_user_profile = '';
  foreach ($user_attributes as $attribute) {
    if(!in_array($attribute->key,
      array('email', 'lastName', 'firstName', 'createdDate', 'lastLogin', 'username')))
    $html_user_profile .=
      theme('obiba_mica_data_access_request_user-profile-attributes', array(
        'label' => t($attribute->key),
        'value' => $attribute->value,
      ));
  }
  if (!empty($html_user_profile)) {
    return $html_user_profile;
  }
  return FALSE;
}

function obiba_mica_data_access_request_users() {
  $mica_client = new MicaDataAccessRequest(NULL, 'METHOD_GET');
  $response = $mica_client->getUsersList();

  drupal_json_output($response);
}

<?php

/**
 * @file
 * MicaStudyResource class
 */

$path_module_protobuf = drupal_get_path('module', 'obiba_protobuf');
$path_module_protos = drupal_get_path('module', 'mica_client_model');

include_once($path_module_protobuf . '/protobuf/Protobuf.php');
use \DrSlump\Protobuf;

Protobuf::autoload();

include_once($path_module_protos . '/protos/Mica.php');
include_once($path_module_protos . '/protos/MicaSearch.php');

/**
 * Class MicaStudyResource
 */
class MicaStudyResource extends MicaClient {

  public function __construct($mica_url = NULL) {
    parent::__construct($mica_url);
    Protobuf::setDefaultCodec(new Protobuf\Codec\Binary);
  }

  public function studies_search($spec_dto_params = NULL) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/studies/_search/?' . 'size=' . parent::SIZE_RESPONSE;
    $url_parm = explode('?', $_SERVER['REQUEST_URI']);
    if (empty($url_parm[1])) {
      $dto_param = $this->dto_filter_construct(NULL, NULL, 'study', $spec_dto_params);
      if (empty(json_decode($dto_param)->filteredQuery->filter)) {
        $request = new HttpClientRequest($url, array(
          'method' => HttpClientRequest::METHOD_GET,
          'headers' => $this->authorizationHeader(array(
                'Accept' => array(parent::HEADER_JSON),
              )
            ),
        ));
      }
      else {
        $request = new HttpClientRequest($url, array(
          'method' => HttpClientRequest::METHOD_POST,
          'headers' => $this->authorizationHeader(array(
                'Accept' => array(parent::HEADER_JSON),
                'Content-Type' => array(parent::HEADER_JSON)
              )
            ),
          'data' => $dto_param,
        ));
      }

    }
    else {
      $dto_param = $this->dto_filter_construct($url_parm[1], array('child' => 'Variable'), 'study', $spec_dto_params);
      if (empty(json_decode($dto_param)->filteredQuery->filter)) {
        $url = $this->micaUrl . '/studies/_search/?from=' . json_decode($dto_param)->from . '&size=' . parent::SIZE_RESPONSE;

        $request = new HttpClientRequest($url, array(
          'method' => HttpClientRequest::METHOD_GET,
          'headers' => $this->authorizationHeader(array(
                'Accept' => array(parent::HEADER_JSON),
              )
            ),
        ));
      }
      else {
        $request = new HttpClientRequest($url, array(
          'method' => HttpClientRequest::METHOD_POST,
          'headers' => $this->authorizationHeader(array(
                'Accept' => array(parent::HEADER_JSON),
                'Content-Type' => array(parent::HEADER_JSON)
              )
            ),
          'data' => $dto_param,
        ));
      }
    }

    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $dto_data_response = json_decode($data);
      $this->session_facets_builder($dto_data_response, 'studies');

      // dpm($dto_data_response);
      $summaries_studies = !empty($dto_data_response->{'obiba.mica.StudyResultDto.result'}->summaries) ?
        $dto_data_response->{'obiba.mica.StudyResultDto.result'}->summaries : NULL;
      $dtoObj = '\obiba\mica\StudySummaryDto';
      $dtos = array();
      $summaries_data = !empty($summaries_studies) ? $summaries_studies : NULL;
      if ($summaries_data) {
        foreach ($summaries_studies as $study) {
          $dtos ['data'][] = Protobuf::decode($dtoObj, json_encode($study));
        }
      }
      $dtos ['totalHits'] = $dto_data_response->totalHits;
      return $dtos;
    } catch (HttpClientException $e) {
      return array();
    }
  }

  public function study($study_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/study/' . $study_id;

    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_BINARY),
          )
        ),
    ));

    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $dtoObj = '\obiba\mica\StudyDto';
      Protobuf::setDefaultCodec(new Protobuf\Codec\Binary);
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    } catch (HttpClientException $e) {
      return array();
    }
  }

}
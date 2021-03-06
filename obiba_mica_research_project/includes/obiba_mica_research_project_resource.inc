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

/**
 * @file
 * Research project resource class
 */

/**
 * Class MicaResearchProjectResource
 */
class MicaResearchProjectResource extends MicaClient {

  /**
   * MicaResearchProjectResource constructor.
   *
   * @param null|string $mica_url
   */
  public function __construct($mica_url) {
    parent::__construct($mica_url);
  }

  /**
   * @param null $query
   * @param int $from
   * @param int $limit
   * @param null $sort
   * @param null $order
   * @return array|mixed
   */
  public function getProjects($query = NULL, $from = 0, $limit = 5, $sort = NULL, $order = NULL) {
    $this->setLastResponse(NULL);
    $params = empty($from) ? 'from=0' : 'from=' . $from;
    $params .= empty($limit) ? '&limit=5' : '&limit=' . $limit;
    $params .= empty($sort) ? '' : '&sort=' . $sort;
    $params .= empty($order) ? '' : '&order=' . $order;
    $params .= empty($query) ? '' : '&query=' . $query;
    $resource_query = '/projects?' . $params;

    $this->drupalCache->MicaGetCache($resource_query);
    if (!isset($this->result_resource_query)) {
      $url_projects = $this->micaUrl . $resource_query;
      $request = new HttpClientRequest($url_projects, array(
        'method' => HttpClientRequest::METHOD_GET,
        'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON)
          )
        )
      ));
      $client = $this->client();
      try {
        $data = $client->execute($request);
        $this->setLastResponse($client->lastResponse);
        $projects = json_decode($data);
        if(!empty($projects->total)){
          $this->result_resource_query = $projects;
          $this->drupalCache->MicaSetCache($resource_query, $this->result_resource_query);
        }

      } catch (HttpClientException $e) {
        watchdog('Mica Client', 'Connection to server fail,  Error serve code : @code, message: @message',
          array(
            '@code' => $e->getCode(),
            '@message' => $e->getMessage()
          ), WATCHDOG_WARNING);
        $this->result_resource_query = NULL;
        return array();
      }
    }
    return $this->result_resource_query;
  }

  /**
   * @return array|mixed
   */
  public function getProjectsWithAccessibleDAR() {
    $this->setLastResponse(NULL);
    $resource_query = '/projects/dar_accessible';
    $url_projects = $this->micaUrl . $resource_query;
    $this->drupalCache->MicaGetCache($resource_query);
    if(!isset($this->result_resource_query)){
      $request = new HttpClientRequest($url_projects, array(
        'method' => HttpClientRequest::METHOD_GET,
        'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON)
          )
        )
      ));
      $client = $this->client();
      try {
        $data = $client->execute($request);
        $this->setLastResponse($client->lastResponse);
        $projects = json_decode($data);
        if(!empty($projects->total)) {
          $this->result_resource_query = $projects;
          $this->drupalCache->MicaSetCache($resource_query, $this->result_resource_query);
        }
      } catch (HttpClientException $e) {
        watchdog('Mica Client', 'Connection to server fail,  Error serve code : @code, message: @message',
          array(
            '@code' => $e->getCode(),
            '@message' => $e->getMessage()
          ), WATCHDOG_WARNING);
        $this->result_resource_query = NULL;
        return array();
      }
    }
    return $this->result_resource_query;
  }

  /**
   * @param $project_id
   * @return array|null
   */
  public function getProject($project_id) {
    $resource_query = '/project/' . rawurlencode($project_id);
    $this->setLastResponse(NULL);
    $this->drupalCache->MicaGetCache($resource_query);
    $url = $this->micaUrl . $resource_query;
    if (!isset($this->result_resource_query)) {
      $request = new HttpClientRequest($url, array(
        'method' => HttpClientRequest::METHOD_GET,
        'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON)
          )
        ),
      ));
      $client = $this->client();
      try {
        $data = $client->execute($request);
        $this->setLastResponse($client->lastResponse);
        $this->result_resource_query = json_decode($data);
        $this->drupalCache->MicaSetCache($resource_query, $this->result_resource_query);
        return $this->result_resource_query;
      } catch (HttpClientException $e) {
        $this->result_resource_query = NULL;
      }
      return array();
      
    }    
    return $this->result_resource_query;
  }

  public function getFormDefinition() {
    $url = $this->micaUrl . '/config/project/form';
    $this->setLastResponse(NULL);
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
          'Accept' => array(parent::HEADER_JSON),
        )
      ),
    ));
    $client = $this->client();
    try {
      $response = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      return json_decode($response);
    } catch (HttpClientException $e) {
      watchdog('Mica Client', 'Connection to server fail,  Error serve code : @code, message: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      drupal_add_http_header('Status', $e->getCode());
      return empty($client->lastResponse->body) ? FALSE : json_decode($client->lastResponse->body);
    }
  }
}
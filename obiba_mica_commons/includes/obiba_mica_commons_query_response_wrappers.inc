<?php

/*
 * @file code to implement response server wrapper
 * */

abstract class AbstractResponseWrapper {
  const STUDY_RESULT_EXTENSION_ID = 'obiba.mica.StudyResultDto.result';
  const NETWORK_RESULT_EXTENSION_ID = 'obiba.mica.NetworkResultDto.result';
  const DATASET_RESULT_EXTENSION_ID = 'obiba.mica.DatasetResultDto.result';
  const VARIABLE_RESULT_EXTENSION_ID = 'obiba.mica.DatasetVariableResultDto.result';

  protected $dto = NULL;
  protected $documentDto = NULL;

  protected function __construct($joinQueryResponseDto) {
    $this->dto = $joinQueryResponseDto;
  }

  function isEmpty() {
    return empty($this->documentDto);
  }

  function getTotalHits() {
    return empty($this->documentDto) ? 0 : $this->documentDto->totalHits;
  }

  function getTotalCount() {
    return empty($this->documentDto) ? 0 : $this->documentDto->totalCount;
  }

  abstract function hasSummaries();

  abstract function getSummaries();
}

class JoinQueryResponseWrapper {
  const VARIABLES = 'variables';
  const DATASETS = 'datasets';
  const STUDIES = 'studies';
  const NETWORKS = 'networks';

  private $dto = NULL;

  function __construct($joinQueryResponseDto) {
    $this->dto = $joinQueryResponseDto;
  }

  function getResponseDto() {
    return $this->dto;
  }

  function getCountStats() {
    return array(
      'study_total_hits' => empty($this->dto->studyResultDto) ? 0 : $this->dto->studyResultDto->totalHits,
      'study_total_count' => empty($this->dto->studyResultDto) ? 0 : $this->dto->studyResultDto->totalCount,
      'variable_total_hits' => empty($this->dto->variableResultDto) ? 0 : $this->dto->variableResultDto->totalHits,
      'variable_total_count' => empty($this->dto->variableResultDto) ? 0 : $this->dto->variableResultDto->totalCount,
      'network_total_hits' => !empty($this->dto->networkResultDto->totalHits) ?
          $this->dto->networkResultDto->totalHits : 0,
      'network_total_count' => !empty($this->dto->networkResultDto->totalCount) ?
          $this->dto->networkResultDto->totalCount : 0,
      'dataset_total_hits' => !empty($this->dto->datasetResultDto->totalHits) ?
          $this->dto->datasetResultDto->totalHits : 0,
      'dataset_total_count' => !empty($this->dto->datasetResultDto->totalCount) ?
          $this->dto->datasetResultDto->totalCount : 0
    );
  }

  function getResponseWrapper($type) {
    switch ($type) {
      case JoinQueryResponseWrapper::VARIABLES:
        return $this->getVariableResponseWrapper();
      case JoinQueryResponseWrapper::DATASETS:
        return $this->getDatasetResponseWrapper();
      case JoinQueryResponseWrapper::STUDIES:
        return $this->getStudyResponseWrapper();
      case JoinQueryResponseWrapper::NETWORKS:
        return $this->getNetworkResponseWrapper();
    }

    throw new InvalidArgumentException("Invalid wrapper type: $type");
  }

  function getVariableResponseWrapper() {
    return new VariableJoinResponseWrapper($this->dto);
  }

  function getDatasetResponseWrapper() {
    return new DatasetJoinResponseWrapper($this->dto);
  }

  function getStudyResponseWrapper() {
    return new StudyJoinResponseWrapper($this->dto);
  }

  function getNetworkResponseWrapper() {
    return new NetworkJoinResponseWrapper($this->dto);
  }
}

class StudyJoinResponseWrapper extends AbstractResponseWrapper {

  private $studyToNetwork = array();

  function __construct($joinQueryResponseDto) {
    parent::__construct($joinQueryResponseDto);
    if (!empty($this->dto) && !empty($this->dto->studyResultDto)) {
      $this->documentDto = $this->dto->studyResultDto;
      $this->networks = $this->mapNetworkDigests();
    }
  }

  function hasSummaries() {
    return !empty($this->documentDto)
    && !empty($this->documentDto->{self::STUDY_RESULT_EXTENSION_ID}) //
    && !empty($this->documentDto->{self::STUDY_RESULT_EXTENSION_ID}->summaries);
  }

  function getSummaries() {
    return $this->hasSummaries() ? $this->documentDto->{self::STUDY_RESULT_EXTENSION_ID}->summaries : array();
  }

  function getNetworkDigests($study_id) {
    $networks = array();
    if (!empty($this->studyToNetwork[$study_id])) {
      $networks = $this->studyToNetwork[$study_id];
    }
    return empty($networks) ? array() : $networks;
  }

  private function mapNetworkDigests() {
    if (!$this->hasSummaries()) {
      return;
    }

    foreach ($this->getSummaries() as $study) {
      $study_id = $study->id;
      $networks = $this->mapNetwork($study_id);
      if (!empty($networks)) {
        $this->studyToNetwork[$study_id] = $networks;
      }
    }
  }

  private function mapNetwork($study_id) {
    $networkDtoWrapper = new NetworkJoinResponseWrapper($this->dto);
    if (!$networkDtoWrapper->isEmpty() && $networkDtoWrapper->hasDigsts()) {
      return array_filter($networkDtoWrapper->getDigests(), //
        function ($network) use ($study_id) {
          $studies = $network->{'obiba.mica.NetworkDigestDto.studies'};
          return !empty($studies) && !empty($studies->ids) && in_array($study_id, $studies->ids);
        }
      );
    }
    else {
      return array();
    }
  }
}

class NetworkJoinResponseWrapper extends AbstractResponseWrapper {

  function __construct($joinQueryResponseDto) {
    parent::__construct($joinQueryResponseDto);
    if (!empty($this->dto) && !empty($this->dto->networkResultDto)) {
      $this->documentDto = $this->dto->networkResultDto;
    }
  }

  function hasDigsts() {
    return !empty($this->documentDto) //
    && !empty($this->documentDto->{self::NETWORK_RESULT_EXTENSION_ID}) //
    && !empty($this->documentDto->{self::NETWORK_RESULT_EXTENSION_ID}->digests);
  }

  function getDigests() {
    return $this->hasDigsts() ? $this->documentDto->{self::NETWORK_RESULT_EXTENSION_ID}->digests : array();
  }

  function hasSummaries() {
    return !empty($this->documentDto) //
    && !empty($this->documentDto->{self::NETWORK_RESULT_EXTENSION_ID}) //
    && !empty($this->documentDto->{self::NETWORK_RESULT_EXTENSION_ID}->networks);
  }

  function getSummaries() {
    return $this->hasSummaries() ? $this->documentDto->{self::NETWORK_RESULT_EXTENSION_ID}->networks : array();
  }
}

class VariableJoinResponseWrapper extends AbstractResponseWrapper {

  function __construct($joinQueryResponseDto) {
    parent::__construct($joinQueryResponseDto);
    if (!empty($this->dto) && !empty($this->dto->variableResultDto)) {
      $this->documentDto = $this->dto->variableResultDto;
    }
  }

  function hasDetails() {
    return !empty($this->documentDto) //
    && !empty($this->documentDto->{self::VARIABLE_RESULT_EXTENSION_ID}) //
    && !empty($this->documentDto->{self::VARIABLE_RESULT_EXTENSION_ID}->variables);
  }

  function getDetails() {
    return $this->hasDetails() ? $this->documentDto->{self::VARIABLE_RESULT_EXTENSION_ID}->variables : array();
  }

  function hasSummaries() {
    return !empty($this->documentDto) //
    && !empty($this->documentDto->{self::VARIABLE_RESULT_EXTENSION_ID}) //
    && !empty($this->documentDto->{self::VARIABLE_RESULT_EXTENSION_ID}->summaries);
  }

  function getSummaries() {
    return $this->hasSummaries() ? $this->documentDto->{self::VARIABLE_RESULT_EXTENSION_ID}->summaries : array();
  }

}

class DatasetJoinResponseWrapper extends AbstractResponseWrapper {

  function __construct($joinQueryResponseDto) {
    parent::__construct($joinQueryResponseDto);
    if (!empty($this->dto) && !empty($this->dto->datasetResultDto)) {
      $this->documentDto = $this->dto->datasetResultDto;
    }
  }

  function hasSummaries() {
    return !empty($this->documentDto) //
    && !empty($this->documentDto->{self::DATASET_RESULT_EXTENSION_ID}) //
    && !empty($this->documentDto->{self::DATASET_RESULT_EXTENSION_ID}->datasets);
  }

  function getSummaries() {
    return $this->hasSummaries() ? $this->documentDto->{self::DATASET_RESULT_EXTENSION_ID}->datasets : array();
  }

}
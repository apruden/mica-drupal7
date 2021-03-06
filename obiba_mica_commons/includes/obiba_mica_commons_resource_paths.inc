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
 * Class definition path server resources.
 */

/**
 * Mica client path helper Class.
 **/
class MicaClientPathHelpers {

  /**
   * Replace the + on the url.
   *
   * @param string $path
   *   The url.
   *
   * @return string
   *   Transformed url.
   */
  public static function replacePlusInUrl($path) {
    return preg_replace('/\+/', '-', $path, 1);
  }

  /**
   * Normalized the modal target to make a jQuery-safe reference.
   *
   * @param $target
   * @return mixed
   */
  public static function normalizeModalTarget($target) {
    return self::replacePlusInUrl(str_replace(' ', '_', preg_replace('/\./', '___', $target)));
  }

  /**
   * The target of a population dce modal.
   *
   * @param $study_id
   * @param $population_id
   * @param $dce
   * @return mixed
   */
  public static function studyPopulationDceModalTarget($study_id, $population_id, $dce) {
    return MicaClientPathHelpers::normalizeModalTarget('#dce-' . $study_id . '-' . $population_id . '-' .
      (is_string($dce) ? $dce : $dce->id));
  }
}

class MicaClientPathProvider {
  const STUDIES = 'mica/studies';
  const STUDY = 'mica/study/{id}';
  const NETWORKS = 'mica/networks';
  const NETWORK = 'mica/network/{id}';
  const DATASETS = 'mica/datasets';
  const STUDY_DATASETS = 'mica/datasets/study-datasets';
  const STUDY_DATASET = 'mica/study-dataset/{id}';
  const HARMONIZATION_DATASETS = 'mica/datasets/harmonization-datasets';
  const HARMONIZATION_DATASET = 'mica/harmonization-dataset/{id}';
  const HARMONIZATION_VARIABLE = 'mica/variable/{id}:{name}:Dataschema';
  const SEARCH = 'mica/search';
  const CROSSTAB = 'mica/variables';
  const COVERAGE = 'mica/coverage';
  const DATA_ACCESS = 'mica/data_access';
  const DATA_ACCESS_REQUEST = 'mica/data_access/request';
  const DATA_ACCESS_HOME = 'mica/data_access/home';
  const DATA_ACCESS_LIST = 'mica/data_access/requests';
  const DATA_ACCESS_USERS_LIST = 'mica/data_access/users';
  const DATA_ACCESS_PROJECTS = 'mica/data_access/projects';
  const PROJECT = 'mica/research/project/{id}';
  const RESEARCH_PROJECT = 'mica/research';
  const RESEARCH_PROJECT_APPROVED = 'mica/research/projects';
  const AGATE_REGISTER = 'agate/register';
  const AGATE_PROFILE = 'agate/user/profile';
  const REPOSITORY_SEARCH = 'mica/repository';

  /**
   * The study detail url.
   *
   * @param string $id
   *   Study id.
   *
   * @return string
   *   The url of the study detail.
   */
  public static function study($id) {
    return preg_replace('/\\{id\\}/', $id, self::STUDY, 1);
  }

  /**
   * The project detail url or link if no text is present.
   * 
   * @param $id
   * @param $text
   * @return mixed
   */
  public static function project($id, $text = NULL) {
    if (empty($text)) {
      global $base_url;
      return $base_url . '/' . preg_replace('/\\{id\\}/', $id, self::PROJECT, 1);
    } else {
      return l($text, preg_replace('/\\{id\\}/', $id, self::PROJECT, 1));
    }
  }

  /**
   * The data acess request detail link
   * 
   * @param $id
   * @return mixed
   */
  public static function data_access_request($id) {
    return l($id, MicaClientPathProvider::DATA_ACCESS_REQUEST, array('fragment' => '/data-access-request/' . $id));
  }

  /**
   * Returns the url to the draft study document
   * 
   * @param string 
   *  Study $id
   * @return string
   */
  public static function study_draft_url($id) {
    return sprintf("%s/#/study/%s", self::get_mica_url(), $id);
  }
  
  /**
   * The network detail url.
   *
   * @param string $id
   *   Network id.
   *
   * @return string
   *   The url of the network detail.
   */
  public static function network($id) {
    return preg_replace('/\\{id\\}/', $id, self::NETWORK, 1);
  }

  /**
   * Returns the url to the draft network document
   *
   * @param string
   *  Study $id
   * @return string
   */
  public static function network_draft_url($id) {
    return sprintf("%s/#/network/%s", self::get_mica_url(), $id);
  }

  /**
   * GEt the dataset dto.
   *
   * @param object $dataset
   *   The dataset dto'
   *
   * @return object
   *   Study/Harmonization dataset.
   */
  public static function dataset($dataset) {
    if (!empty($dataset->variableType)) {
      return $dataset->variableType == 'Study' ? self::studyDataset($dataset->id) : self::harmonizationDataset($dataset->id);
    }
    if (!empty($dataset->_extensions)) {
      if ($dataset->hasExtension('obiba.mica.StudyDatasetDto.type')) {
        return self::studyDataset($dataset->id);
      }
      if ($dataset->hasExtension('obiba.mica.HarmonizationDatasetDto.type')) {
        return self::harmonizationDataset($dataset->id);
      }
    }

    return self::studyDataset($dataset->id);
  }

  /**
   * Returns the url to the draft study/harmonization dataset document
   *
   * @param string
   *  Study $id
   * @return string
   */
  public static function dataset_draft_url($dataset) {
      return sprintf("%s/#/%s/%s",
          self::get_mica_url(),
          $dataset->variableType == 'Study' ? 'study-dataset' : 'harmonization-dataset',
          $dataset->id);
  }

  /**
   * The study dataset detail url.
   *
   * @param string $id
   *   Study dataset id.
   *
   * @return string
   *   The url of the study dataset detail.
   */
  public static function studyDataset($id) {
    return preg_replace('/\\{id\\}/', $id, self::STUDY_DATASET, 1);
  }

  /**
   * The harmonization dataset detail url.
   *
   * @param string $id
   *   Harmonization dataset id.
   *
   * @return string
   *   The url of the harmonization dataset detail.
   */
  public static function harmonizationDataset($id) {
    return preg_replace('/\\{id\\}/', $id, self::HARMONIZATION_DATASET, 1);
  }

  /**
   * GEt the variable dataset.
   *
   * @param object $variable
   *   The variable dto.
   *
   * @return object
   *   Study/Harmonization variable dataset.
   */
  public static function variableDataset($variable) {
    return $variable->variableType == 'Study' ? self::studyDataset($variable->datasetId) : self::harmonizationDataset($variable->datasetId);
  }

  /**
   * The harmonized variable detail url.
   *
   * @param object $variable
   *   Harmonized variable entity wrapper.
   *
   * @return string
   *   The url of the harmonized variable detail.
   */
  public static function variableHarmonized($variable) {
    return preg_replace(
      '/(\\{id\\}):(\\{name\\})/',
      "$variable->datasetId:$variable->name",
      self::HARMONIZATION_VARIABLE,
      1
    );
  }

  /**
   * Get the public URL if defined in the server otherwise get the mica url as defined in drupal.
   *
   * @return null
   */
  private static function get_mica_url() {
    $mica_config = new MicaConfigResource();
    $url = $mica_config->getPublicURL();
    return empty($url) ? variable_get_value('mica_url') : $url;
  }

}

class MicaClientAnchorHelper {
  const DEFAULT_PRIMARY_BUTTON_CLASSES = 'btn highlight btn btn-primary';

  /**
   * Truncate a given long string.
   *
   * @param string $text
   *   The text link.
   * @param string $paragraph
   *   The paragraph to ellipses.
   * @param string $url
   *   The url to complete paragraph.
   * @param int $max_length
   *   The max length of paragraph to display.
   *
   * @return string
   *   Truncated text.
   */
  public static function ellipses($text, $paragraph, $url, $max_length = 300) {
    if (empty($paragraph)) {
      return '';
    }

    if (drupal_strlen($paragraph) >= $max_length) {
      return text_summary(strip_tags(obiba_mica_commons_markdown($paragraph)), 'html', $max_length) . '... ' . l($text, $url);
    }

    return $paragraph;
  }

  /**
   * Study list item.
   *
   * @param object $study
   *   The study wrapper.
   *
   * @return string
   *   Formatted list.
   */
  public static function studyListItem($study) {
    return self::listItem($study, MicaClientPathProvider::study($study->id));
  }

  /**
   * The study detail url.
   *
   * @param object $study
   *   Study wrapper.
   *
   * @return string
   *   The url of the study detail.
   */
  public static function study($study) {
    $name = obiba_mica_commons_get_localized_field($study, 'acronym');
    return l($name, MicaClientPathProvider::study($study->id), array());
  }

  /**
   * The population detail url.
   *
   * @param object $population
   *   Population wrapper.
   *
   * @return string
   *   The url of the population detail.
   */
  public static function studyPopulationModal($population) {
    return l(obiba_mica_commons_get_localized_field($population, 'name'), '#', array(
      'external' => TRUE,
      'attributes' => array(
        'data-toggle' => 'modal',
        'data-target' => MicaClientPathHelpers::normalizeModalTarget('#population-' . $population->id),
      ),
    ));
  }

  /**
   * The DCE anchor for a study.
   *
   * @param string $study_id
   *   The study id.
   * @param string $population_id
   *   The population id.
   * @param object $dce
   *   The dce wrapper.
   *
   * @return string
   *   The anchor link.
   */
  public static function studyPopulationDceModal($study_id, $population_id, $dce) {
    return l(obiba_mica_commons_get_localized_field($dce, 'name'), '#', array(
      'external' => TRUE,
      'attributes' => array(
        'data-toggle' => 'modal',
        'data-target' => MicaClientPathHelpers::studyPopulationDceModalTarget($study_id, $population_id, $dce),
      ),
    ));
  }

  /**
   * Study dataset anchor link.
   *
   * @param string $text
   *   The text anchor.
   * @param string $study_id
   *   Th study id.
   * @param string $css_class
   *   the affected css class.
   *
   * @return string
   *   The anchor link.
   */
  public static function studyDatasets($text, $study_id, $css_class = NULL) {
      return ObibaSearchResources::repositoryAnchor(
        MicaClientPathProvider::REPOSITORY_SEARCH,
        $text,
        array('class' => ($css_class ? $css_class : '')),
        array(
          'type' => 'datasets',
          'query' => 'study(in(Mica_study.id,' . $study_id . '))',
          'display' => 'list'
        ));
  }

  /**
   * Study variable anchor link.
   *
   * @param string $text
   *   The text anchor.
   * @param string $study_id
   *   The study id.
   * @param bool $is_button
   *   As link or button rendering.
   * @param string $supplementary_query
   * @param string $css_class
   *   the affected css class.
   * @return string The anchor link.
   * The anchor link.
   */
  public static function studyVariables($text, $study_id, $is_button = FALSE, $supplementary_query = NULL, $css_class = NULL) {
      $query = 'study(in(Mica_study.id,' . $study_id . '))';
      if (!empty($supplementary_query)) {
        $query .= ',' . $supplementary_query;
      }
      return ObibaSearchResources::repositoryAnchor(
        MicaClientPathProvider::REPOSITORY_SEARCH,
        empty($text) ? variable_get_value('variables_search_label') : $text,
        array('class' => $is_button ? ($css_class ? $css_class : self::DEFAULT_PRIMARY_BUTTON_CLASSES) : ''),
        array(
          'type' => 'variables',
          'query' => $query,
          'display' => 'list'
        ));
  }

  /**
   * Dce study variable anchor link.
   *
   * @param string $text
   *   The text anchor.
   * @param string $dce_id
   *   The dce id.
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function dceStudyVariables($text, $dce_id, $is_button = FALSE) {
      return ObibaSearchResources::repositoryAnchor(
        MicaClientPathProvider::REPOSITORY_SEARCH,
        empty($text) ? variable_get_value('variables_search_label') : $text,
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'type' => 'variables',
          'query' => 'variable(in(Mica_variable.dceIds,' . urlencode($dce_id) . '))',
          'display' => 'list'
        ));
  }

  /**
   * Dce study coverage anchor link.
   *
   * @param string $dce_id
   *   The dce id.
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function coverageDceStudy($dce_id, $is_button = FALSE) {
      return ObibaSearchResources::repositoryAnchor(
        MicaClientPathProvider::REPOSITORY_SEARCH,
        variable_get_value('variable_coverage_label'),
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'type' => 'variables',
          'query' => 'variable(in(Mica_variable.dceIds,' . urlencode($dce_id) . '))',
          'display' => 'coverage'
        ));
  }

  /**
   * Network list item.
   *
   * @param object $network
   *   The network wrapper.
   *
   * @return string
   *   Formatted list.
   */
  public static function networkListItem($network) {
    return self::listItem($network, MicaClientPathProvider::network($network->id));
  }

  /**
   * Network detail url.
   *
   * @param object $network
   *   The network wrapper.
   *
   * @return string
   *   Formatted list.
   */
  public static function network($network) {
    $name = obiba_mica_commons_get_localized_field($network, 'acronym');
    return l($name, MicaClientPathProvider::network($network->id), array());
  }

  /**
   * Get link to networks by study id.
   *
   * @param string $text
   *   The test link.
   * @param string $study_id
   *   The study id.
   * @param string $css_class
   *   the affected css class.
   *
   * @return string
   *   The anchor link.
   */
  public static function studyNetworks($text, $study_id, $css_class) {
      return ObibaSearchResources::repositoryAnchor(
        MicaClientPathProvider::REPOSITORY_SEARCH,
        $text,
        array('class' => ($css_class ? $css_class : '')),
        array(
          'type' => 'networks',
          'query' => 'network(in(Mica_network.studyIds,' . $study_id . '))',
          'display' => 'list'
        ));

  }

  /**
   * Get link to list of studies by networks id.
   *
   * @param string $text
   *   Text link.
   * @param string $network_id
   *   The network id.
   * @param array $attributes
   *   A set of link attributes.
   *
   * @return string
   *   The anchor link.
   */
  public static function networkStudies($text, $network_id, array $attributes = array()) {
      return ObibaSearchResources::repositoryAnchor(
        MicaClientPathProvider::REPOSITORY_SEARCH,
        empty($text) ? variable_get_value('variables_search_label') : $text,
        $attributes,
        array(
          'type' => 'studies',
          'query' => 'network(in(Mica_network.id,' . $network_id . '))',
          'display' => 'list'
        ));
  }

  /**
   * Get link to list of datasets by networks id.
   *
   * @param string $text
   *   Text link.
   * @param string $network_id
   *   The network id.
   *
   * @return string
   *   The anchor link.
   */
  public static function networkDatasets($text, $network_id, array $attributes = array()) {
      return ObibaSearchResources::repositoryAnchor(
        MicaClientPathProvider::REPOSITORY_SEARCH,
        empty($text) ? variable_get_value('variables_search_label') : $text,
        $attributes,
        array(
          'type' => 'datasets',
          'query' => 'network(in(Mica_network.id,' . $network_id . '))',
          'display' => 'list'
        ));
  }

  /**
   * Get link to list of variables by networks id.
   *
   * @param string $text
   *   Text link.
   * @param string $network_id
   *   The network id.
   * @param array $attributes
   *   A set of link attributes.
   * @param string $supplementary_query
   * @return string The anchor link.
   * The anchor link.
   */
  public static function networkVariables($text, $network_id, array $attributes = array(), $supplementary_query = NULL) {
      $query = 'network(in(Mica_network.id,' . $network_id . '))';
      if (!empty($supplementary_query)) {
        $query .= ',' . $supplementary_query;
      }
      return ObibaSearchResources::repositoryAnchor(
        MicaClientPathProvider::REPOSITORY_SEARCH,
        empty($text) ? variable_get_value('variables_search_label') : $text,
        $attributes,
        array(
          'type' => 'variables',
          'query' => $query,
          'display' => 'list'
        ));
  }

  /**
   * Get link to search networks.
   *
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function searchNetworks($is_button = FALSE) {
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        variable_get_value('networks_search_label'),
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'type' => 'networks',
          'query' => 'network(exists(Mica_network.id))',
          'display' => 'list'
        ));
  }

  /**
   * The network coverage anchor link.
   *
   * @param string $network_id
   *   The network id.
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function coverageNetwork($network_id, $is_button = FALSE) {
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        variable_get_value('variable_coverage_label'),
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'type' => 'networks',
          'query' => 'network(in(Mica_network.id,' . $network_id . '))',
          'display' => 'coverage',
          'bucket' => 'study'
        ));
  }

  /**
   * The networks coverage anchor link.
   *
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function coverageNetworks($is_button = FALSE) {
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        variable_get_value('variable_coverage_label'),
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'type' => 'networks',
          'query' => 'network(exists(Mica_network.id))',
          'display' => 'coverage'
        ));
  }

  /**
   * Dataset list item.
   *
   * @param object $dataset
   *   The study wrapper.
   *
   * @return string
   *   Formatted list.
   */
  public static function datasetListItem($dataset) {
    return self::listItem($dataset, MicaClientPathProvider::dataset($dataset));
  }

  /**
   * Cross tab dataset link.
   *
   * @param object $dataset
   *   Dataset wrapper.
   *
   * @return string
   *   The anchor link.
   */
  public static function datasetCrosstab($dataset) {
    $res = l(
      t('Crosstabs'),
      MicaClientPathProvider::CROSSTAB,
      array(
        'attributes' => array('class' => self::DEFAULT_PRIMARY_BUTTON_CLASSES),
        'fragment' => preg_replace('/mica\\//', '/crosstab/', MicaClientPathProvider::dataset($dataset)),
      )
    );

    return $res;
  }

  /**
   * Get link to list of variables by dataset id.
   *
   * @param string $text
   *   Text link.
   * @param string $dataset_id
   *   The dataset id.
   * @param array $attributes
   *   A set of link attributes.
   * @param string $supplementary_query
   *   A supplementary query.
   *
   * @return string
   *   The anchor link.
   */
  public static function datasetVariables($text, $dataset_id, array $attributes = array(), $supplementary_query = NULL) {
      $query = '';
      if (!empty($supplementary_query)) {
        $query .= ',' . $supplementary_query;
      }
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        empty($text) ? variable_get_value('variables_search_label') : $text,
        $attributes,
        array(
          'type' => 'variables',
          'query' => 'dataset(in(Mica_dataset.id,' . $dataset_id . '))' . $query,
          'display' => 'list'
        ));
  }

  /**
   * The dataset coverage anchor link.
   *
   * @param string $dataset_id
   *   The dataset id.
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function coverageDataset($dataset_id, $is_button = FALSE) {
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        variable_get_value('variable_coverage_label'),
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'type' => 'variables',
          'query' => 'dataset(in(Mica_dataset.id,' . $dataset_id . '))',
          'display' => 'coverage'
        ));
  }

  /**
   * Get link to list of studies by dataset id.
   *
   * @param string $text
   *   Text link.
   * @param string $dataset_id
   *   The dataset id.
   * @param array $attributes
   *   A set of link attributes.
   *
   * @return string
   *   The anchor link.
   */
  public static function datasetStudies($text, $dataset_id, array $attributes = array()) {
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        $text,
        $attributes,
        array(
          'type' => 'studies',
          'query' => 'dataset(in(Mica_dataset.id,' . $dataset_id . '))',
          'display' => 'list'
        ));
  }

  /**
   * Get link to list of networks by dataset id.
   *
   * @param string $text
   *   Text link.
   * @param string $dataset_id
   *   The dataset id.
   * @param array $attributes
   *   A set of link attributes.
   *
   * @return string
   *   The anchor link.
   */
  public static function datasetNetworks($text, $dataset_id, array $attributes = array()) {
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        $text,
        $attributes,
        array(
          'type' => 'networks',
          'query' => 'dataset(in(Mica_dataset.id,' . $dataset_id . '))',
          'display' => 'list'
        ));
  }

  /**
   * A search button studies.
   *
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function searchStudies($is_button = FALSE) {
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        variable_get_value('studies_search_label'),
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'type' => 'studies',
          'query' => 'study(exists(Mica_study.id))',
          'display' => 'list'
        ));
  }

  /**
   * The studies coverage anchor link.
   *
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function coverageStudies($is_button = FALSE) {
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        variable_get_value('variable_coverage_label'),
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'query' => 'study(exists(Mica_study.id))',
          'display' => 'coverage'
        ));
  }

  /**
   * The study coverage anchor link.
   *
   * @param string $study_id
   *   The study id.
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function coverageStudy($study_id, $is_button = FALSE) {
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        variable_get_value('variable_coverage_label'),
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'query' => 'study(in(Mica_study.id,' . $study_id . '))',
          'display' => 'coverage'
        ));
  }

  /**
   * The datasets coverage anchor link.
   *
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function coverageDatasets($is_button = FALSE, $bucket = 'dataset') {
      $label = t('Harmonization Dataset');
      if ($bucket == 'dataset') {
        $label = t('Study Dataset');
      }
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        variable_get_value('variable_coverage_label') . ' ' . t('by') . ' ' . $label,
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'type' => 'datasets',
          'query' => 'dataset(exists(Mica_dataset.id))',
          'display' => 'coverage',
          'bucket' => $bucket
        ));
  }

  /**
   * A search button datasets.
   *
   * @param bool $is_button
   *   As link or button rendering.
   *
   * @return string
   *   The anchor link.
   */
  public static function searchDatasets($is_button = FALSE) {
      return ObibaSearchResources::repositoryAnchor(MicaClientPathProvider::REPOSITORY_SEARCH,
        variable_get_value('datasets_search_label'),
        array('class' => $is_button ? self::DEFAULT_PRIMARY_BUTTON_CLASSES : ''),
        array(
          'type' => 'datasets',
          'query' => 'dataset(exists(Mica_dataset.id))',
          'display' => 'list'
        ));
  }

  /**
   * Get dataset list link by variable.
   *
   * @param object $variable
   *   The variable wrapper.
   *
   * @return string
   *   The anchor link.
   */
  public static function variableDataset($variable) {
    $dataset_name = obiba_mica_commons_get_localized_field($variable, 'datasetName');
    return l(
      $dataset_name,
      MicaClientPathProvider::variableDataset($variable)
    );
  }


  /**
   * Cross tab variable link.
   *
   * @param object $variable
   *   Dataset wrapper.
   *
   * @return string
   *   The anchor link.
   */
  public static function variableCrosstab($variable) {
    $dataset = $variable->variableType === 'Study' ? MicaClientPathProvider::studyDataset($variable->datasetId) : MicaClientPathProvider::harmonizationDataset($variable->datasetId);

    $res = l(
      t('Crosstabs'),
      MicaClientPathProvider::CROSSTAB,
      array(
        'attributes' => array('class' => self::DEFAULT_PRIMARY_BUTTON_CLASSES),
        'fragment' => preg_replace('/mica\\//', '/crosstab/', $dataset) . '/variable/' . $variable->id,
      )
    );

    return $res;
  }

  /**
   * Get an harmonized variable link.
   *
   * @param object $variable
   *   The variable wrapper.
   *
   * @return string
   *   The anchor link.
   */
  public static function variableHarmonized($variable) {
    return l(
      $variable->name,
      MicaClientPathProvider::variableHarmonized($variable)
    );
  }

  /**
   * Get list item of given entities.
   *
   * @param object $dto
   *   The entity Dto (study, variable, network, ...)
   * @param string $url
   *   The provided entity detail path.
   *
   * @return string
   *   The anchor link.
   */
  public static function listItem($dto, $url) {
    $acronym = obiba_mica_commons_get_localized_field($dto, 'acronym');
    $name = obiba_mica_commons_get_localized_field($dto, 'name');
    $title_order_option = variable_get_value('study_title_acronym_display');
    if (!empty($title_order_option)) {
      if ($title_order_option == 1) {
        return l($acronym == $name ? $acronym : $acronym . '  -  ' . $name, $url);
      }
      if ($title_order_option == 2) {
        return l($name, $url);
      }

    }
    return l($acronym == $name ? $acronym : $name . '  -  ' . $acronym, $url);

  }

  /**
   * Construct an ajax friendly anchor.
   *
   * @param string $url
   *   The provided entity detail path.
   * @param string $text
   *   The text link.
   * @param array $attributes
   *   A set of given attributes.
   * @param array $query_params
   *   Query parameters.
   *
   * @return string
   *   The anchor link.
   */
  public static function ajaxFriendlyAnchor($url, $text, array $attributes, array $query_params) {
    $options = array();

    // Drupal may require css classes to be inside an array!
    if (!empty($attributes) && !empty($attributes['class'])) {
      $attributes['class'] = self::formatClassAsArray($attributes['class']);
    }

    if (!empty($attributes)) {
      $options['attributes'] = $attributes;
    }
    if (!empty($query_params)) {
      if (empty($query_params['query'])) {
        unset($query_params['query']);
      }
      $options['fragment'] = '!' . http_build_query($query_params);
    }

    return l($text, $url, $options);
  }

  /**
   * Format class as array.
   *
   * @param string $class
   *   A class to add.
   *
   * @return array
   *   A formatted class array.
   */
  public static function formatClassAsArray($class) {
    return is_array($class) ? $class : explode(' ', $class);
  }

}

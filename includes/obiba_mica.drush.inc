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
 * drush commands  implementations to help users download required libraries.
 */

/**
 * Implements hook_drush_command().
 */
function obiba_mica_drush_command() {
  $items['download-mica-dependencies'] = array(
    'callback' => 'obiba_mica_drush_download_js_dependencies',
    'aliases' => array('dlmdeps'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Download Mica JavaScript dependencies for the production environment.'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries</code>).'),
    ),
  );

  $items['mica-study-timeline-download'] = array(
    'callback' => 'obiba_mica_drush_timeline_download',
    'aliases' => array('dhtime'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Downloads the ths study timeline library from timeline repositories.'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries/mica-study-timeline</code>).'),
    ),
  );

  $items['highcharts-download'] = array(
    'callback' => 'obiba_mica_drush_highcharts_download',
    'aliases' => array('dhili'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Downloads the Optional highcharts library from highcharts repositories.'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries/highcharts</code>).'),
    ),
  );

  $items['datatables-plugins-download'] = array(
    'callback' => 'obiba_mica_drush_datatable_bootstrap_plugins_download',
    'aliases' => array('dataplg'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Downloads the Optional Datatables Bootstrap Plugin library from datatables.net .'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries/highcharts</code>).'),
    ),
  );

  $items['datatables-download'] = array(
    'callback' => 'obiba_mica_drush_datatable_download',
    'aliases' => array('ddlib'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Downloads the Optional Datatables library from datatables.net .'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries/datatables</code>).'),
    ),
  );

  $items['obiba-progressbar-download'] = array(
    'callback' => 'obiba_mica_drush_obiba_progressbar',
    'aliases' => array('opblib'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Downloads the  obiba-progressbar library from Obiba repository .'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (<code>sites/all/libraries/obiba-progressbar-1.0.0</code>).'),
    ),
  );

  return $items;
}

/**
 * Implementation of command to download timeline library.
 */
function obiba_mica_drush_timeline_download() {
  $args = func_get_args();
  if ($args[1]) {
    $path = $args[1];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  }
  // Create the path if it does not exist.
  if (!is_dir($path . '/sites/all')) {
    drush_op('mkdir', $path . '/sites/all');
  }
  if (!is_dir($path . '/sites/all/libraries')) {
    drush_op('mkdir', $path . '/sites/all/libraries');
  }
  $path = $path . '/sites/all/libraries';
  chdir($path);

  if ($args[0]) {
    $version_time_line = $args[0];
  }
  else {
    $version_time_line = variable_get('obiba-time-line-version', '1.0');
  }
  $svn_cmd = 'wget  https://github.com/obiba/mica-study-timeline/archive/' . $version_time_line . '.zip';
  if (drush_shell_exec($svn_cmd)) {
    drush_log(dt('mica-study-timeline  was downloaded to !path.', array('!path' => ' ' . $path)), 'success');
    $unzip_cmd = 'unzip ' . $version_time_line . '.zip -d .';
    if (drush_shell_exec($unzip_cmd)) {
      drush_log(dt('mica-study-timeline Unzipped  to !path.', array('!path' => 'mica-study-timeline-master')), 'success');
      $timeline_lib_path = 'mica-study-timeline-' . $version_time_line;
      chdir($timeline_lib_path);
      $cmd_get_d3 = 'bower --allow-root install ; cp bower_components/d3/d3.min.js dist/  ; mv ../' . $version_time_line . '.zip bower_components/';
      if (drush_shell_exec($cmd_get_d3)) {
        drush_log(dt('d3 js downloaded and copied !path.', array('!path' => 'mica-study-timeline/dist')), 'success');
        $cmd_remove_bower_components = "rm -rf bower_components";
        if (drush_shell_exec($cmd_remove_bower_components)) {
          drush_log(dt('bower_components folder and source zip removed.'), 'success');
          drush_log(dt('Timeline library successfully installed'), 'success');
        }
      }

    }
  }
  else {
    drush_log(dt('Drush was unable to download mica-study-timeline to !path.', array('!path' => ' ' . $path)) . '<br/>' . dt('Attempted command: !wget.', array('!wget' => '<code>' . $svn_cmd . '</code>')), 'error');
  }

}

/**
 * Implementation of command to download HighCharts library.
 */
function obiba_mica_drush_highcharts_download() {
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  }
  // Create the path if it does not exist.
  if (!is_dir($path . '/sites/all')) {
    drush_op('mkdir', $path . '/sites/all');
  }
  if (!is_dir($path . '/sites/all/libraries')) {
    drush_op('mkdir', $path . '/sites/all/libraries');
  }
  if (!is_dir($path . '/sites/all/libraries/highcharts')) {
    drush_op('mkdir', $path . '/sites/all/libraries/highcharts');
  }
  if (!is_dir($path . '/sites/all/libraries/highcharts/js')) {

    drush_op('mkdir', $path . '/sites/all/libraries/highcharts/js');
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }
  else {
    drush_log('No dir @path', array('@path' => $path), 'notice');
  }
  $path = $path . '/sites/all/libraries/highcharts/js';
  chdir($path);

  $svn_cmd = 'wget   http://code.highcharts.com/highcharts.js';
  if (drush_shell_exec($svn_cmd)) {
    drush_log(dt('Highcharts.js  was downloaded to !path.', array('!path' => ' ' . $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download Highcharts.js to !path.', array('!path' => ' ' . $path)) . '<br/>' . dt('Attempted command: !wget.', array('!wget' => '<code>' . $svn_cmd . '</code>')), 'error');
  }

  $svn_cmd = 'wget   http://code.highcharts.com/highcharts-more.js';
  if (drush_shell_exec($svn_cmd)) {
    drush_log(dt('Highcharts-more.js  was downloaded to !path.', array('!path' => ' ' . $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download Highcharts-more.js to !path.', array('!path' => ' ' . $path)) . '<br/>' . dt('Attempted command: !wget.', array('!wget' => '<code>' . $svn_cmd . '</code>')), 'error');
  }

}

/**
 * Implementation of command to download dataTable library.
 */
function obiba_mica_drush_datatable_download() {
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  }

  // Create the libraries path if it does not exist.
  if (!is_dir($path . '/sites/all')) {
    drush_op('mkdir', $path . '/sites/all');
  }
  if (!is_dir($path . '/sites/all/libraries')) {
    drush_op('mkdir', $path . '/sites/all/libraries');
  }
  if (!is_dir($path . '/sites/all/libraries/datatables')) {
    drush_op('mkdir', $path . '/sites/all/libraries/datatables');
  }
  $path = $path . '/sites/all/libraries/datatables';
  drush_op('mkdir', $path . '/DataTables-master');
  chdir($path . '/DataTables-master');

  $get_cmd = 'wget http://datatables.net/releases/DataTables-1.10.4.zip';
  if (drush_shell_exec($get_cmd)) {
    drush_log(dt('DataTables  was downloaded to !path.', array('!path' => ' ' . $path)), 'success');

    $unzip_cmd = 'unzip DataTables-1.10.4.zip -d ' . $path . '/DataTables-master';
    if (drush_shell_exec($unzip_cmd)) {
      drush_log(dt('DataTables Unzipped  to !path.', array('!path' => ' ' . $path)), 'success');
      $path_to_del = $path . '/DataTables-master/DataTables-1.10.4';
      chdir($path_to_del);
      $mv_cmd = "mv * ../../";
      if (drush_shell_exec($mv_cmd)) {
        drush_log(dt('DataTables archive moved to   !path.', array('!path' => ' ' . $path)), 'success');
        chdir($path);
        $rm_cmd = "rm " . '-rf ' . $path . '/DataTables-master';
        if (drush_shell_exec($rm_cmd)) {
          drush_log(dt('DataTables archive deleted !path.', array('!path' => ' ' . $path)), 'success');
        }
      }
    }
  }
}

/**
 * Implementation of command to download DataTable Bootstrap Plugin library.
 */
function obiba_mica_drush_datatable_bootstrap_plugins_download() {
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  }

  if (!is_dir($path . '/sites/all/libraries/datatables')) {
    drush_log(dt('Drush was unable to find DataTables plugin ', array()), 'error');
  }
  else {
    $path = $path . '/sites/all/libraries/datatables/extensions';
    chdir($path);
    // Create the plugins path if it does not exist.
    if (!is_dir($path . '/Plugins')) {
      drush_op('mkdir', $path . '/Plugins');
    }

    $path = $path . '/Plugins';
    drush_op('mkdir', $path . '/Plugins-master');
    chdir($path . '/Plugins-master');
    $get_cmd = 'wget https://github.com/DataTables/Plugins/archive/master.zip';

    if (drush_shell_exec($get_cmd)) {

      $unzip_cmd = 'unzip master.zip -d ' . $path . '/Plugins-master';
      if (drush_shell_exec($unzip_cmd)) {
        $path_to_del = $path . '/Plugins-master/Plugins-master';
        chdir($path_to_del);
        $mv_cmd = "mv * ../../";
        if (drush_shell_exec($mv_cmd)) {
          chdir($path);
          $rm_cmd = "rm " . '-rf ' . $path . '/Plugins-master';
          if (drush_shell_exec($rm_cmd)) {
            drush_log(dt('DataTables archive deleted !path.', array('!path' => ' ' . $path)), 'success');
          }
        }
      }

      drush_log(dt('DataTables Plugins was downloaded to !path.', array('!path' => ' ' . $path)), 'success');
    }
    else {
      drush_log(dt('Drush was unable to download DataTables Plugins to !path.', array('!path' => ' ' . $path)) . '<br/>' . dt('Attempted command: !git.', array('!git' => '<code>' . $get_cmd . '</code>')), 'error');
    }
  }
}

/**
 * Implementation of command to download Obiba Progress bar library.
 */
function obiba_mica_drush_obiba_progressbar() {
  $args = func_get_args();
  if ($args[1]) {
    $path = $args[1];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  }

  if ($args[0]) {
    $version_progress_bar = $args[0];
  }
  else {
    $version_progress_bar = variable_get('obiba-progressbar-lib-version', '1.0.0');
  }

  // Create the libraries path if it does not exist.
  if (!is_dir($path . '/sites/all')) {
    drush_op('mkdir', $path . '/sites/all');
  }
  if (!is_dir($path . '/sites/all/libraries')) {
    drush_op('mkdir', $path . '/sites/all/libraries');
  }
  if (!is_dir($path . "/sites/all/libraries/obiba-progressbar-$version_progress_bar")) {
    drush_op('mkdir', $path . "/sites/all/libraries/obiba-progressbar-$version_progress_bar");
  }
  $path = $path . "/sites/all/libraries/obiba-progressbar-$version_progress_bar";
  drush_op('mkdir', $path . '/obiba-progressbar-master');
  chdir($path . '/obiba-progressbar-master');

  $get_cmd = "wget https://github.com/obiba/obiba-progressbar/archive/$version_progress_bar.zip";
  if (drush_shell_exec($get_cmd)) {
    drush_log(dt('Obiba Progressbar  was downloaded to !path.', array('!path' => ' ' . $path)), 'success');

    $unzip_cmd = "unzip $version_progress_bar.zip -d " . $path . '/obiba-progressbar-master';
    if (drush_shell_exec($unzip_cmd)) {
      drush_log(dt('Obiba Progressbar Unzipped  to !path.', array('!path' => ' ' . $path)), 'success');
      $path_to_del = $path . "/obiba-progressbar-master/";
      chdir($path_to_del);
      $mv_cmd = "mv obiba-progressbar-$version_progress_bar/* ../";
      if (drush_shell_exec($mv_cmd)) {
        drush_log(dt('Obiba Progressbar archive moved to   !path.', array('!path' => ' ' . $path)), 'success');
        chdir($path);
        $rm_cmd = "rm " . '-rf ' . $path . '/obiba-progressbar-master';
        if (drush_shell_exec($rm_cmd)) {
          drush_log(dt('Obiba Progressbar archive deleted !path.', array('!path' => ' ' . $path)), 'success');
        }
      }
    }
  }
}

/**
 * Uses curl to install all required JS packages. No other dependencies than
 * Curl is required.
 */
function obiba_mica_drush_download_js_dependencies(){
 $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  }

  if ($args[1]) {
    $repository = $args[1];
  }
  else {
    $repository = variable_get('mica-drupal-js-libraries-version', '1.2.3');
  }

  $path .= '/sites/all/libraries';

  // Create the libraries path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
  }

  // clean up content
  if (!drush_shell_exec("rm -rf $path/*")) {
    drush_log(dt("Failed to empty $path content."), 'error');
    return;
  }

  $repository_name = 'mica-drupal-js-libraries';
  $dependencies_url = 'https://github.com/obiba/' . $repository_name . '/archive/' . $repository . '.tar.gz';

  if (!drush_shell_exec("rm -rf $path/*")) {
    drush_log(dt("Failed to empty $path content."), 'error');
    return;
  }

  if (!drush_shell_exec("curl -Ls $dependencies_url |  tar -xzf - -C $path/. --strip-components=1")) {
    drush_log(dt("Failed download to dependencies from $dependencies_url to $path."), 'error');
    return;
  }
  else {
    drush_log(dt("Dependencies downloaded from $dependencies_url to $path."), 'success');
  }

  if (!drush_shell_exec("rm -rf .git*")) {
    drush_log(dt("Failed to delete Git related file"), 'warning');
  }
}

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
 * Obiba Mica variable file
 */

/**
 * Implements hook_variable_info().
 */
function obiba_mica_persons_variable_info($options) {
  $variable['associated_people'] = array(
    'title' => t('Affiliated Members'),
    'description' => t('The Affiliated Members captions'),
    'type' => 'string',
    'default' => 'Affiliated Members',
  );
  return $variable;
}

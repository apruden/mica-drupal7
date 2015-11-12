<?php
/**
 * @file
 * Code for the obiba_mica_persons modules.
 */

?>
<!--
  ~ Copyright (c) 2015 OBiBa. All rights reserved.
  ~
  ~ This program and the accompanying materials
  ~ are made available under the terms of the GNU Public License v3.0.
  ~
  ~ You should have received a copy of the GNU General Public License
  ~ along with this program.  If not, see <http://www.gnu.org/licenses/>.
  -->

<div id="associated-people" class="modal fade"
  xmlns="http://www.w3.org/1999/html">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal"
          aria-hidden="true">&times;</button>
        <h3
          class="modal-title"><?php print variable_get_value('associated_people'); ?></h3>
      </div>
      <div class="modal-body">
        <button id="download-csv-person" class="btn btn-success pull-right"><i
            class="glyphicon glyphicon-download"></i><?php print t('Download'); ?>
        </button>
        <?php if (!empty($persons_table)): ?>
          <?php print $persons_table; ?>
        <?php endif; ?>
      </div>
      <div class="clearfix"></div>
      <div class="modal-footer">
        <button type="button" class="btn btn-default"
          data-dismiss="modal"><?php print t('Close'); ?></button>
      </div>
    </div>
  </div>
</div>

<?php
//dpm($coverages);
//dpm($vocabulary_coverage_outputs);
//dpm($query);
//dpm($group_by);
?>

  <div id="search-query"></div>
  <div>
    <p class="pull-left">
      <?php print t('%hits variables (among %count)', array(
        '%hits' => $coverages->totalHits,
        '%count' => $coverages->totalCount
      )) ?>
      <?php
      print l(t('Search'), 'mica/search', array(
        'attributes' => array(
          'class' => array(
            'btn',
            'btn-primary',
            'indent'
          )
        ),
        'query' => array(
          'type' => 'variables',
          array('query' => $query)
        ),
      )); ?>
    </p>

    <ul class="nav nav-pills pull-right">
      <li class="<?php if (empty($group_by)) print 'active' ?>">
        <?php
        print l(t('All'), 'mica/coverage', array(
          'query' => array(
            array('query' => $query)
          ),
        )); ?>
      </li>
      <li class="<?php if (!empty($group_by) && $group_by == 'studyIds') print 'active' ?>" data-toggle="tooltip"
          data-placement="top" title="<?php print t('Group by study') ?>">
        <?php
        print l(t('Study'), 'mica/coverage', array(
          'query' => array(
            array(
              'query' => $query,
              'group-by' => 'studyIds'
            )
          ),
        )); ?>
      </li>
      <li class="<?php if (!empty($group_by) && $group_by == 'datasetId') print 'active' ?>" data-toggle="tooltip"
          data-placement="top" title="<?php print t('Group by dataset') ?>">
        <?php
        print l(t('Dataset'), 'mica/coverage', array(
          'query' => array(
            array(
              'query' => $query,
              'group-by' => 'datasetId'
            )
          ),
        )); ?>
      </li>
    </ul>
  </div>

<?php $has_coverage = FALSE; ?>

  <article class="pull-left">
    <?php foreach ($coverages->taxonomies as $taxonomy_coverage) : ?>
      <?php if (!empty($taxonomy_coverage->hits) && !empty($taxonomy_coverage->vocabularies)): ?>

        <section>
<!--          --><?php //if (!empty($taxonomy_coverage->buckets)): ?>
<!--            <div class="btn-group pull-right">-->
<!--              <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">-->
<!--                --><?php //print t('Display'); ?><!-- <span class="caret"></span>-->
<!--              </button>-->
<!--              <ul class="dropdown-menu" role="menu">-->
<!--                <li><a href="#">--><?php //print t('All'); ?>
<!--                    <div class="pull-right help-inline">--><?php //print $taxonomy_coverage->hits; ?><!--</div>-->
<!--                  </a></li>-->
<!--                <li class="divider"></li>-->
<!--                --><?php //foreach ($taxonomy_coverage->buckets as $bucket): ?>
<!--                  <li><a href="#--><?php //print $bucket->value; ?><!--">--><?php //print $bucket->value; ?>
<!--                      <div class="pull-right help-inline">--><?php //print $bucket->hits; ?><!--</div>-->
<!--                    </a>-->
<!--                  </li>-->
<!--                --><?php //endforeach ?>
<!--              </ul>-->
<!--            </div>-->
<!--          --><?php //endif ?>

          <h3>
            <?php print mica_client_commons_get_localized_field($taxonomy_coverage->taxonomy, 'titles'); ?>
          </h3>

          <p class="help-block">
            <?php print mica_client_commons_get_localized_field($taxonomy_coverage->taxonomy, 'descriptions'); ?>
          </p>
          <?php foreach ($taxonomy_coverage->vocabularies as $vocabulary_coverage) : ?>
            <?php if (!empty($vocabulary_coverage->hits)): ?>
              <?php $has_coverage = TRUE; ?>
            <?php endif ?>
            <?php print render($vocabulary_coverage_outputs[$taxonomy_coverage->taxonomy->name][$vocabulary_coverage->vocabulary->name]); ?>
          <?php endforeach; ?>
        </section>
      <?php endif ?>
    <?php endforeach; ?>
  </article>

<?php if (!$has_coverage): ?>
  <p class="md-top-margin pull-left">
    <i><?php print t('No coverage'); ?></i>
  </p>
<?php endif ?>
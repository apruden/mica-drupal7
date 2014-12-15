<div class="menu-toggle btn btn-default glyphicon glyphicon-chevron-right" id="menu-toggle">
</div>
<nav id="scroll-menu" data-spy="affix">
  <ul class="nav">
    <li>
      <a href="#overview">
        <?php print t('Overview / Design'); ?>
      </a>
    </li>
    <li>
      <a href="#access">
        <?php if (!empty($study_dto->markerPaper) || !empty($study_dto->pubmedId)): ?>
          <?php print t('Access / Marker Paper'); ?>
        <?php else: ?>
          <?php print t('Access'); ?>
        <?php endif; ?>
      </a>
    </li>
    <?php if (!empty($study_dto->info)): ?>
      <li>
        <a href="#info"><?php print t('Supplementary Information'); ?>
        </a>
      </li>
    <?php endif; ?>
    <?php if (!empty($study_dto->attachments)): ?>
      <li>
        <a href="#documents">
          <?php print t('Documents'); ?>
        </a>
      </li>
    <?php endif; ?>
    <li>
      <a href="#timeline">
        <?php print t('Timeline'); ?>
      </a>
    </li>
    <li>
      <a href="#populations">
        <?php print t('Populations'); ?>
      </a>
    </li>
    <?php if (!empty($coverage)): ?>
      <li>
        <a href="#coverage">
          <?php print t('Classifications Coverage'); ?>
        </a>
      </li>
    <?php endif; ?>
    <?php if (!empty($networks)): ?>
      <li>
        <a href="#networks">
          <?php print t('Networks'); ?>
        </a>
      </li>
    <?php endif; ?>
    <?php if (!empty($datasets)): ?>
      <li>
        <a href="#datasets">
          <?php print t('Datasets'); ?>
        </a>
      </li>
    <?php endif; ?>
    <?php if (!empty($datasets) && !empty($study_variables_aggs)): ?>
      <li>
        <a href="#variables">
          <?php print t('Variables'); ?>
        </a>
      </li>
    <?php endif; ?>
  </ul>
</nav>
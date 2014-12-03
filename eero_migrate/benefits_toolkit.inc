<?php

/**
 * Migration class to import Benefits Toolkit data from Eero into nodes.
 */

class BenefitsToolkitsMigration extends Migration {
  public function __construct() {
    // Always call the parent constructor first for basic setup.
    parent::__construct();

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'not null' => TRUE,
          'alias' => 'bt',
         )
      ),
      MigrateDestinationNode::getKeySchema(),
      'eero_migration'
    );

    // Table: lps_factoids
    $lps_factoids_fields = array(
      'id',
      'toolkit',
      'title',
    );

    $this->description = t('Migrate Benefits Toolkits from Eero to Drupal nodes.');
    $query = Database::getConnection('default', 'eero_migration')
             ->select('lps_toolkit_items', 'bt')
             ->fields('bt', $lps_factoids_fields);

    $query->orderBy('bt.id', 'ASC');

    $this->source = new MigrateSourceSQL($query, $this->fields());
    $this->source->setMapJoinable(FALSE);
    $this->destination = new MigrateDestinationNode('benefits_toolkit_page');

    $this->addSimpleMappings(array('title'));

    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('status')->defaultValue(1);

    $this->addFieldMapping('body', 'toolkit');
    $this->addFieldMapping('body:format')
         ->defaultValue('text_links_files');

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'id',
      'the_date',
    ));
  }

  public function fields() {
    return array(
      'toolkit' => t('Toolkit'),
      'title' => t('Title'),
    );
  }
}

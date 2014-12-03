<?php

/**
 * Migration class to import Fast Facts data from Eero into nodes.
 */

class FastFactsMigration extends Migration {
  public function __construct() {
    // Always call the parent constructor first for basic setup.
    parent::__construct();

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array(
          'type' => 'int',
          'not null' => TRUE,
          'alias' => 'ff',
         )
      ),
      MigrateDestinationNode::getKeySchema(),
      'eero_migration'
    );

    // Table: lps_factoids
    $lps_factoids_fields = array(
      'id',
      'factoid',
      'citation',
    );

    $this->description = t('Migrate Fast Facts from Eero to Drupal nodes.');
    $query = Database::getConnection('default', 'eero_migration')
             ->select('lps_factoids', 'ff')
             ->fields('ff', $lps_factoids_fields);

    $query->orderBy('ff.id', 'ASC');

    $this->source = new MigrateSourceSQL($query, $this->fields());
    $this->source->setMapJoinable(FALSE);
    $this->destination = new MigrateDestinationNode('fast_fact_page');

    $this->addSimpleMappings(array('title'));

    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('status')->defaultValue(1);

    $this->addFieldMapping('body', 'factoid');
    $this->addFieldMapping('field_citation', 'citation');
    $this->addFieldMapping('field_citation:format')
         ->defaultValue('text_links');

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'id',
      'the_date',
    ));
  }

  public function prepareRow($current_row) {
    $current_row->factoid = strip_tags($current_row->factoid);
    $current_row->title = text_summary($current_row->factoid, 'plain_text', 50);
  }

  public function fields() {
    return array(
      'factoid' => t('Fast Fact'),
      'citation' => t('Citation'),
    );
  }
}

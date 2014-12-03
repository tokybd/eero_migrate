<?php

/**
 * Migration class to import Case Study Briefs data from Eero into nodes.
 */

class CaseStudyBriefsMigration extends Migration {
  public function __construct() {
    // Always call the parent constructor first for basic setup.
    parent::__construct();

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'page_id' => array(
          'type' => 'int',
          'not null' => TRUE,
          'alias' => 'm',
         )
      ),
      MigrateDestinationNode::getKeySchema(),
      'eero_migration'
    );

    // Table: my_pages
    $my_pages_fields = array(
      'page_id',
      'page_title',
      'page_nav_title',
      'page_description',
      'page_keywords',
      'page_published',
    );

    // Table: lps_case_study
    $lps_case_study_fields = array(
      'full_name',
      'email_address',
      'primary_phone_number',
      'designers',
      'project_team',
      'project_address_1',
      'project_address_2',
      'project_city',
      'project_state',
      'project_country',
      'project_zip',
      'project_size',
      'budget',
      'completion_date',
      'role_of_architect',
      'references_and_awards',
      'overview',
      'problem',
      'solution',
      'sustainable_features',
      'cost_comparison',
      'lessons_learned',
    );

    $this->description = t('Migrate Case Study Briefs from Eero to Drupal nodes.');
    $query = Database::getConnection('default', 'eero_migration')
             ->select('my_pages', 'm')
             ->fields('m', $my_pages_fields)
             ->condition('page_published', '1', '=')
             ->condition('page_template_id', '31', '=');

    $query->join('lps_case_study', 'c', 'c.page_id = m.page_id');
    $query->fields('c', $lps_case_study_fields)
          ->condition('submission_status', '3', '=');

    $query->orderBy('m.page_id', 'ASC');

    $this->source = new MigrateSourceSQL($query, $this->fields());
    $this->source->setMapJoinable(FALSE);
    $this->destination = new MigrateDestinationNode('case_study_brief_page');

    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('is_new')->defaultValue(TRUE);

    $this->addFieldMapping('nid', 'page_id');
    $this->addFieldMapping('status', 'page_published');
    $this->addFieldMapping('title', 'page_title');
    $this->addFieldMapping('metatag_description', 'page_description');
    $this->addFieldMapping('metatag_keywords', 'page_keywords');

    $this->addFieldMapping('field_longtext12', 'designers');
    $this->addFieldMapping('field_text2', 'project_size');
    $this->addFieldMapping('field_text3', 'budget');
    $this->addFieldMapping('field_text4', 'completion_date');

    $this->addFieldMapping('field_location')->defaultValue(TRUE);
    $this->addFieldMapping('field_location:street', 'project_address_1');
    $this->addFieldMapping('field_location:additional', 'project_address_2');
    $this->addFieldMapping('field_location:city', 'project_city');
    $this->addFieldMapping('field_location:province', 'project_state');
    $this->addFieldMapping('field_location:country', 'project_country');
    $this->addFieldMapping('field_location:postal_code', 'project_zip');

    $this->addFieldMapping('body', 'overview');
    $this->addFieldMapping('body:format')
         ->defaultValue('text_links_h4');
    $this->addFieldMapping('field_longtext', 'sustainable_features');
    $this->addFieldMapping('field_longtext:format')
         ->defaultValue('text_links_h4');
    $this->addFieldMapping('field_longtext2', 'problem');
    $this->addFieldMapping('field_longtext2:format')
         ->defaultValue('text_links_h4');
    $this->addFieldMapping('field_longtext3', 'solution');
    $this->addFieldMapping('field_longtext3:format')
         ->defaultValue('text_links_h4');
    $this->addFieldMapping('field_longtext5', 'cost_comparison');
    $this->addFieldMapping('field_longtext5:format')
         ->defaultValue('text_links_h4');
    $this->addFieldMapping('field_longtext6', 'lessons_learned');
    $this->addFieldMapping('field_longtext6:format')
         ->defaultValue('text_links_h4');
    $this->addFieldMapping('field_longtext8', 'project_team');
    $this->addFieldMapping('field_longtext8:format')
         ->defaultValue('text_links_h4');
    $this->addFieldMapping('field_longtext9', 'role_of_architect');
    $this->addFieldMapping('field_longtext9:format')
         ->defaultValue('text_links_h4');

    $this->addFieldMapping('field_first_name', 'page_nav_title');
    $this->addFieldMapping('field_last_name', 'full_name');
    $this->addFieldMapping('field_email', 'email_address');
    $this->addFieldMapping('field_phone_number', 'primary_phone_number');

    $this->addFieldMapping('field_longtext11', 'references_and_awards');
    $this->addFieldMapping('field_longtext11:format')
         ->defaultValue('ul_text_links');

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'changed',
      'promote',
      'sticky',
      'revision',
      'language',
      'revision_uid',
      'log',
      'tnid',
      'translate',
      'body:summary',
      'body:language',
      'comment',
    ));
  }

  public function prepareRow($current_row) {
    $parts = explode(' ', $current_row->full_name);
    if ($parts) {
      $current_row->page_nav_title = array_shift($parts);
      $current_row->full_name = implode(' ', $parts);
    }

    include_once DRUPAL_ROOT . '/includes/iso.inc';
    $countries = _country_get_predefined_list();

    if (FALSE !== ($key = array_search($current_row->project_country, $countries))) {
      $current_row->project_country = $key;
    }
    else {
      $current_row->project_country = NULL;
    }

    return TRUE;
  }

  public function fields() {
    return array(
      'page_id' => t('Page ID'),
      'page_title' => t('Page Title'),
      'page_nav_title' => t('Page Nav Title'),
      'page_description' => t('Page Description'),
      'page_keywords' => t('Page Keywords'),
      'page_published' => t('Page Published'),
      'full_name' => t('Full Name'),
      'email_address' => t('Email Address'),
      'primary_phone_number' => t('Primary Phone Number'),
      'designers' => t('Designers'),
      'project_team' => t('Project Team'),
      'project_address_1' => t('Project Address 1'),
      'project_address_2' => t('Project Address 2'),
      'project_city' => t('Project City'),
      'project_state' => t('Project State'),
      'project_country' => t('Project Country'),
      'project_zip' => t('Project Zip'),
      'project_size' => t('Project Size'),
      'budget' => t('Budget'),
      'completion_date' => t('Completion Date'),
      'role_of_architect' => t('Role of Architect'),
      'references_and_awards' => t('References and Awards'),
      'overview' => t('Overview'),
      'problem' => t('Problem'),
      'solution' => t('Solution'),
      'sustainable_features' => t('Sustainable Features'),
      'cost_comparison' => t('Cost Comparison'),
      'lessons_learned' => t('Lessons Learned'),
    );
  }
}

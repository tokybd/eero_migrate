<?php
/**
 * @file
 * Declare the API being used for the migration of Eero content into Drupal.
 */

/*
 * Implements hook_migrate_api().
 */
function eero_migrate_migrate_api() {
  $api = array(
    'api' => 2,
    'migrations' => array(
      'BenefitsToolkits' => array('class_name' => 'BenefitsToolkitsMigration'),
      'CaseStudyBriefs' => array('class_name' => 'CaseStudyBriefsMigration'),
      'FastFacts' => array('class_name' => 'FastFactsMigration'),
    ),
  );
  return $api;
}

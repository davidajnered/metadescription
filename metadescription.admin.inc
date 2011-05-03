<?php

/**
 * Page callback for settings page.
 */
function metadescription_settings_page() {
  drupal_add_css(drupal_get_path('module', 'metadescription') . '/metadescription.css');
  $output = drupal_get_form('metadescription_add_description_form');
  $output .= drupal_get_form('metadescription_descriptions_form');
  return $output;
}

/**
 * Form to add new meta descriptions
 */
function metadescription_add_description_form($form_state) {
  $form['title'] = array(
    '#type' => 'item',
    '#value' => '<h3>' . t('Add a new description') . '</h3>',
  );
  $form['add-descriptions'] = array(
    '#type' => 'fieldset',
  );
  $form['add-descriptions']['uri'] = array(
    '#type' => 'textfield',
    '#title' => 'URI',
    '#required' => TRUE,
    '#description' => "Use % as a wildcard for dynamically generated uri's",
  );
  $form['add-descriptions']['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Description',
    '#required' => TRUE,
    '#description' => "Use [number] as placeholder to retrieve values from the uri. The number should be the part of the uri you want to use (zero-index).",
  );
  $form['add-descriptions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['add-descriptions']['example'] = array(
     '#type' => 'item',
     '#title' => 'Example',
     '#description' => '<strong>URI:</strong> /jobs/melbourne<br><strong>Description:</strong> Find [0] in [1]<br><strong>Text in &lt;head&gt;:</strong> Find jobs in melbourne',
   );
  return $form;
}

/**
 * Implementation of hook_validate().
 */
function metadescription_add_description_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if(empty($values['uri']) || empty($values['description'])) {
    form_set_error('', t('No field can be empty'));
  }
}

/**
 * Implementation of hook_submit().
 */
function metadescription_add_description_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $data = array(
    'uri' => '/' . trim($values['uri'], '/'),
    'description' => $values['description'],
  );
  drupal_write_record('metadescription', $data);
}

/**
 * Form to list saved meta descriptions
 * Todo: improve and clean up
 */
function metadescription_descriptions_form() {
  $form = array();
  $form['form'] = array(
    '#prefix' => '<table id="metadescription-list-form"><tr><th></th><th>URI</th><th>Description</th></tr>',
    '#suffix' => '</table>',
  );
  $form['form']['title'] = array(
    '#type' => 'item',
    '#value' => '<h3>Descriptions</h3>',
  );
  /*$form['form']['head'] = array(
    '#type' => 'item',
    '#value' => '<tr><th></th><th>URI</th><th>Description</th></tr>',
  );*/

  $res = db_query('SELECT * FROM {metadescription} ORDER BY mid DESC');
  while($row = db_fetch_array($res)) {
    $form['form']['mid-' . $row['mid']] = array(
      '#type' => 'checkbox',
      '#prefix' => '<tr><td>',
      '#suffix' => '</td><td>' . $row['uri'] . '</td><td>' . $row['description'] . '</td></tr>'
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  return $form;
}

/**
 * Implementation of hook_menu().
 */
function metadescription_descriptions_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  foreach($values as $key => $value) {
    if(strpos($key, 'mid-') === 0 && $value == 1) {
      $mid = (int)str_replace('mid-', '', $key);
      db_query('DELETE FROM {metadescription} WHERE mid = ' . $mid);
      $delete = TRUE;
    }
  }
  if($delete) {
    drupal_set_message('Description deleted');
  }
}
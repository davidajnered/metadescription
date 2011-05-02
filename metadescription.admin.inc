<?php

function metadescription_admin_form($form_state) {
  $form['uri'] = array(
    '#type' => 'textfield',
    '#title' => 'URI',
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => 'Description',
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function metadescription_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if(empty($values['uri']) || empty($values['description'])) {
    form_set_error('', t('No field can be empty'));
  }
}

function metadescription_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $data = array(
    'uri' => '/' . trim($values['uri'], '/'),
    'description' => $values['description'],
  );
  drupal_write_record('metadescription', $data);
}

function metadescription_admin_page() {
  drupal_add_css(drupal_get_path('module', 'metadescription') . '/metadescription.css');
  $output = drupal_get_form('metadescription_admin_form');
  $output .= drupal_get_form('metadescription_list_form');
  return $output;
}

function metadescription_list_form() {
  $res = db_query('SELECT * FROM {metadescription} ORDER BY mid DESC');
  $form = array();
  $form['form'] = array(
    '#prefix' => '<table id="metadescription-list-form">',
    '#suffix' => '</table>'
  );
  $form['form']['head'] = array(
    '#type' => 'item',
    '#value' => '<tr><th></th><th>URI</th><th>Description</th></tr>',
  );
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

function metadescription_list_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  foreach($values as $key => $value) {
    if(strpos($key, 'mid-') === 0 && $value == 1) {
      $mid = (int)str_replace('mid-', '', $key);
      db_query('DELETE FROM {metadescription} WHERE mid = ' . $mid);
    }
  }
  drupal_set_message('Description deleted');
}
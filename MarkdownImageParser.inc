<?php

/**
 * Class for Parsing ImageField shorthand into Markdown
 */

module_load_include('php', 'markdown');

class MarkdownImageFieldParser extends Markdown_Parser {
  protected $entity_type;
  protected $entity;
  protected $field_names;

  public function __construct($entity_type, $entity, $field_names) {
    $this->entity_type = $entity_type;
    $this->entity = $entity;
    $this->field_names = $field_names;

    parent::__construct();
  }

  public function _doImages_inline_callback($matches) {
    if ($matches[3] == '') {
      $match_index = 4;
    }
    else {
      $match_index = 3;
    }

    $original = $matches[0];
    $url = $matches[$match_index];

    // Ignore valid URLs (i.e. http://example.com/image.jpg and /image.jpg)
    // as they are never image shortcuts
    if (valid_url($url, TRUE)) {
      return $original;
    }

    $shortcut = explode(':', $url);
    $field_name = $shortcut[0];

    // Ignore inline references that don't refer to image fields.
    if (!in_array($field_name, $this->field_names)) {
      return $original;
    }

    list($id, $vid, $bundle) = entity_extract_ids($this->entity_type, $this->entity);

    $parts = array(
      'module' => 'markdown_image',
      'entity_type' => $this->entity_type,
      'entity id' => $id,
      'field_name' => $field_name,
      'index' => $shortcut[1] - 1,
      'alt' => $matches[2],
    );
    return sprintf('[%s]', implode(':', $parts));
  }
}

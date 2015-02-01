<?php
namespace aquagear\text\tags;

class ImageTag extends CoreTag {
	private $src = '';
	
	public function init($value) {
		$this->src = $value['src'];
	}
	
	protected function prev() {
		return '<img src="'.$this->src.'">';
	}
	
	protected function fin() {
		return '</img>';
	}
}
?>
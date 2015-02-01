<?php
namespace aquagear\text\tags;

class LinkTag extends CoreTag {
	private $src = '';
	
	public function init($value) {
		$this->src = $value['src'];
	}
	
	protected function prev() {
		return '<a href="'.$this->src.'">';
	}
	
	protected function fin() {
		return '</a>';
	}
}
?>
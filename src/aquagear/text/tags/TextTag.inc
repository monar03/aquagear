<?php 
namespace aquagear\text\tags;

class TextTag extends CoreTag {
	private $str = '';
	
	public function init($str) {
		$this->str = $str;
	}
		
	protected function prev() {
		return $this->str;
	}
	
	protected function fin() {
		return '';
	}
	
}
?>
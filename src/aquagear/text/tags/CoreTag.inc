<?php 
namespace aquagear\text\tags;

abstract class CoreTag extends \aquagear\Object {
	private $obj_stack = array();
	
	abstract protected function prev();
	abstract protected function fin();
	abstract public function init($value);
	
	public function execute() {
		$str = '';
		$str .= $this->prev();
		foreach($this->obj_stack as $obj) {
			if($obj instanceof \aquagear\text\tags\CoreTag) {
				$str .= $obj->execute();
			}
		}
		$str .= $this->fin();
		return $str;
	}
	
	public function pushObj($obj) {
		$this->obj_stack[] = $obj;
	}
}
?>
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema results_development
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `results_development` ;

-- -----------------------------------------------------
-- Schema results_development
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `results_development` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `results_development` ;

-- -----------------------------------------------------
-- Table `results_development`.`results`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `results_development`.`results` ;

CREATE TABLE IF NOT EXISTS `results_development`.`results` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `analytic_instance_id` INT NULL,
  `timestamp` VARCHAR(60) NULL,
  `result_text` VARCHAR(1000) NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
